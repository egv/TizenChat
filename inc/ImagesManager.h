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
    Tizen::Graphics::Bitmap* GetBitmapForUrl(Tizen::Base::String url,
    										 int width,
    										 int height,
    										 void* listener,
    										 Tizen::Base::Collection::HashMap* userInfo);

	virtual void OnImageDecodeUrlReceived(RequestId reqId, Tizen::Graphics::Bitmap* pBitmap, result r, const Tizen::Base::String errorCode, const Tizen::Base::String errorMessage);


private:
    Tizen::Base::String __cacheDirName;

    ImagesManager();
    ImagesManager(ImagesManager const&);
    void operator=(ImagesManager const&);


};

#endif /* IMAGESMANAGER_H_ */
