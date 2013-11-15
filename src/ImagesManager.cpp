/*
 * ImagesManager.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#include "IImagesManagerDelegate.h"
#include "ImagesManager.h"

#include "Utils.h"

#import <FApp.h>
#import <FBase.h>
#import <FIo.h>

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::Io;
using namespace Tizen::App;
using namespace Tizen::Media;
using namespace Tizen::Graphics;

ImagesManager::ImagesManager()
{
	// determine cache dir name
	__cacheDirName = String(App::GetInstance()->GetAppDataPath() + L"images_cache");

	// See if cache dir exists and if not - create it
	if (!File::IsFileExist(__cacheDirName))
	{
		// create it if not
		Directory *pDir = new Directory();
		pDir->Construct(__cacheDirName);
		pDir->Create(__cacheDirName, true);

		delete pDir;
	}

	// Init image decoding
	__pImage = new Image;
	__pImage->Construct();

	// Init delegates pool
	__pListenersHashMap = new HashMap;
	__pListenersHashMap->Construct();

	__pUserInfoHashMap = new HashMap;
	__pUserInfoHashMap->Construct();
}


Tizen::Graphics::Bitmap*
ImagesManager::GetUnknownAvatar()
{
	return Utils::getInstance().GetBitmapWithName(String(L"no_photo_user.png"));
}

// get bitmap from disk or from network
Tizen::Graphics::Bitmap*
ImagesManager::GetBitmapForUrl(const Tizen::Base::String url,
							   IImagesManagerDelegate* listener,
							   Tizen::Base::Collection::HashMap* userInfo)
{
	Bitmap *pBitmap;

	String fileName;
	FileNameForUrl(url, fileName);

	String fullFileName(__cacheDirName);
	fullFileName.Append(L"/");
	fullFileName.Append(fileName);

	if (File::IsFileExist(fullFileName))
	{
		pBitmap = __pImage->DecodeN(fullFileName, BITMAP_PIXEL_FORMAT_ARGB8888);
	}
	else
	{
		pBitmap = GetUnknownAvatar();

		RequestId reqId;
		Uri uri;
		uri.SetUri(url);

		__pImage->DecodeUrl(uri, BITMAP_PIXEL_FORMAT_ARGB8888, 108, 108, reqId, *this, 60000);

		__pListenersHashMap->Add(new Integer(reqId), listener);
		__pUserInfoHashMap->Add(new Integer(reqId), ConstructRequestHashMap(fullFileName, userInfo));
	}

    return pBitmap;
}

void
ImagesManager::RemoveListener(IImagesManagerDelegate* listener)
{
	if (__pListenersHashMap == null)
	{
		return;
	}

	ArrayList *pArray = new ArrayList;
	pArray->Construct();

    IMapEnumerator* pMapEnum = __pListenersHashMap->GetMapEnumeratorN();
    Object* pKey = null;
    Object* pValue = null;
    while (pMapEnum->MoveNext() == E_SUCCESS)
    {
        pKey = pMapEnum->GetKey();
        pValue = pMapEnum->GetValue();
        if (pValue == listener)
        {
        	pArray->Add(pKey);
        }
    }
    delete pMapEnum;

    IEnumerator* pEnum = pArray->GetEnumeratorN();
    while (pEnum->MoveNext() == E_SUCCESS)
    {
    	__pListenersHashMap->Remove(*(pEnum->GetCurrent()));
    }

    delete pEnum;
    delete pArray;
}

void
ImagesManager::FileNameForUrl(const String url, String& fileName)
{
	fileName.Format(100, L"%d.png", url.GetHashCode());
}

void
ImagesManager::OnImageDecodeUrlReceived(RequestId reqId, Tizen::Graphics::Bitmap* pBitmap, result r, const Tizen::Base::String errorCode, const Tizen::Base::String errorMessage)
{
	AppLogDebug("image dl finished. req: %d, r: %d, errorCode: %S, errorMessage: %S", reqId, r, errorCode.GetPointer(), errorMessage.GetPointer());
	IImagesManagerDelegate* listener = null;
	HashMap* userInfo = null;
	HashMap* requestInfo = null;
	String* fileName = null;

	Integer key(reqId);

	if (__pListenersHashMap->ContainsKey(key))
	{
		listener = static_cast<IImagesManagerDelegate*>(__pListenersHashMap->GetValue(key));
		__pListenersHashMap->Remove(key);
	}

	if (__pUserInfoHashMap->ContainsKey(key))
	{
		requestInfo = static_cast<HashMap*>(__pUserInfoHashMap->GetValue(key));
		__pUserInfoHashMap->Remove(key);
		userInfo = static_cast<HashMap*>(requestInfo->GetValue(String(L"userInfo")));
		fileName = static_cast<String*>(requestInfo->GetValue(String(L"fileName")));
	}

	if (!IsFailed(r))
	{
		// IMG_FORMAT_PNG
		if (fileName != null)
		{
			__pImage->EncodeToFile(*pBitmap, IMG_FORMAT_PNG, *fileName, true);
		}

		if (listener != null)
		{
			listener->OnImageManagerDownloadedImage(pBitmap, userInfo);
		}
	}
	else
	{
		if (listener != null)
		{
			listener->OnImageManagerDownloadFailed(userInfo);
		}
	}
}

Tizen::Base::Collection::HashMap*
ImagesManager::ConstructRequestHashMap(Tizen::Base::String fileName, Tizen::Base::Collection::HashMap* userInfo)
{
	HashMap* pHashMap = new HashMap;
	pHashMap->Construct(2);

	pHashMap->Add(new String(L"fileName"), new String(fileName));
	pHashMap->Add(new String(L"userInfo"), userInfo);

	return pHashMap;
}

