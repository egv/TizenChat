/*
 * ImagesManager.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#include "ImagesManager.h"

#import <FApp.h>
#import <FBase.h>
#import <FIo.h>

using namespace Tizen::Base;
using namespace Tizen::Io;
using namespace Tizen::App;
using namespace Tizen::Media;
using namespace Tizen::Graphics;

ImagesManager::ImagesManager()
{
	// here we need to do the following
	// check if there is a directory
	__cacheDirName = String(App::GetInstance()->GetAppDataPath() + L"images_cache");

	if (!File::IsFileExist(__cacheDirName))
	{
		// create it if not
		Directory *pDir = new Directory();
		pDir->Construct(__cacheDirName);
		pDir->Create(__cacheDirName, true);

		delete pDir;
	}
}

// get bitmap from disk or from network
Tizen::Graphics::Bitmap*
ImagesManager::GetBitmapForUrl(Tizen::Base::String url,
							   int width,
							   int height,
							   void* listener,
							   Tizen::Base::Collection::HashMap* userInfo)
{
    AppResource *pAppResource = App::GetInstance()->GetAppResource();
    Bitmap* result = null;

    if (pAppResource)
    {
        Bitmap* pBitmap = pAppResource->GetBitmapN(L"no_photo_user.png", BITMAP_PIXEL_FORMAT_ARGB8888);
    	Bitmap* pMaskBitmap = pAppResource->GetBitmapN(L"thumbnail_list.png", BITMAP_PIXEL_FORMAT_ARGB8888);

    	if (pBitmap)
        {
    		Canvas *pCanvas = new Canvas();
    		pCanvas->Construct(Rectangle(0, 0, 108, 108));
    		pCanvas->DrawBitmap(Rectangle(0, 0, 108, 108), *pBitmap, Rectangle(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight()));
    		pCanvas->DrawBitmap(Rectangle(0, 0, 108, 108), *pMaskBitmap, Rectangle(0, 0, pMaskBitmap->GetWidth(), pMaskBitmap->GetHeight()));

    		result = new Bitmap();
    		result->Construct(*pCanvas, Rectangle(0, 0, 108, 108));
        }
    }

    return result;
}

void
ImagesManager::OnImageDecodeUrlReceived(RequestId reqId, Tizen::Graphics::Bitmap* pBitmap, result r, const Tizen::Base::String errorCode, const Tizen::Base::String errorMessage)
{

}
