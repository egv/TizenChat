/*
 * IImagesManagerDelegate.h
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#ifndef IIMAGESMANAGERDELEGATE_H_
#define IIMAGESMANAGERDELEGATE_H_

#include <FBase.h>
#include <FGraphics.h>

class IImagesManagerDelegate : public Tizen::Base::Object
{
public:
	virtual ~IImagesManagerDelegate() {};

	virtual void OnImageManagerDownloadedImage(Tizen::Graphics::Bitmap* pBitmap, Tizen::Base::Collection::HashMap* userInfo) = 0;
	virtual void OnImageManagerDownloadFailed(Tizen::Base::Collection::HashMap* userInfo) = 0;
};

#endif /* IIMAGESMANAGERDELEGATE_H_ */
