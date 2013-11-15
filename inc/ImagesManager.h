/*
 * ImagesManager.h
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#ifndef IMAGESMANAGER_H_
#define IMAGESMANAGER_H_

#include <FBase.h>
#include <FGraphics.h>
#include <FMedia.h>

#include "IImagesManagerDelegate.h"

class ImagesManager : public Tizen::Media::IImageDecodeUrlEventListener
{
public:
    static ImagesManager& GetInstance()
    {
        static ImagesManager instance;
        return instance;
    };

    ~ImagesManager() {};

    // get bitmap from disk or from network
    Tizen::Graphics::Bitmap* GetUnknownAvatar();
    Tizen::Graphics::Bitmap* GetBitmapForUrl(Tizen::Base::String url,
    										 IImagesManagerDelegate* listener,
    										 Tizen::Base::Collection::HashMap* userInfo);

    void RemoveListener(IImagesManagerDelegate* listener);

    virtual void OnImageDecodeUrlReceived(RequestId reqId, Tizen::Graphics::Bitmap* pBitmap, result r, const Tizen::Base::String errorCode, const Tizen::Base::String errorMessage);

private:
    Tizen::Base::String __cacheDirName;
    Tizen::Media::Image* __pImage;
    Tizen::Base::Collection::HashMap* __pUserInfoHashMap;
    Tizen::Base::Collection::HashMap* __pListenersHashMap;

    ImagesManager();
    ImagesManager(ImagesManager const&);
    void operator=(ImagesManager const&);

    void FileNameForUrl(const Tizen::Base::String url, Tizen::Base::String& fileName);

    Tizen::Base::Collection::HashMap* ConstructRequestHashMap(Tizen::Base::String fileName, Tizen::Base::Collection::HashMap* userInfo);
};

#endif /* IMAGESMANAGER_H_ */
