#include <binder/IPCThreadState.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/CameraSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/OMXClient.h>
#include <media/stagefright/OMXCodec.h>
#include <camera/Camera.h>
#include <camera/CameraParameters.h>
#include <gui/Surface.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <cutils/properties.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <utils/Log.h>
#include <binder/IServiceManager.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <StagefrightRecorder.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ui/GraphicBuffer.h>
#include <camera/ICamera.h>
#include <camera/ICameraClient.h>
#include <camera/ICameraService.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <utils/KeyedVector.h>
#include <utils/Log.h>
#include <utils/Vector.h>
#include <utils/threads.h>
#include <media/stagefright/SurfaceMediaSource.h>
#include <gui/SurfaceComposerClient.h>

using namespace android;

OMXClient client;
CameraParameters *mCameraParams;

sp<Camera> camera;
sp<ICamera> mCamera;
sp<ICameraService> cs;
sp<Surface> mPreviewSurface;
sp<IGraphicBufferProducer> gbp;
sp<CameraSource> cameraSource;
sp<SurfaceControl> mSurfaceControl;
sp<ICameraRecordingProxy> mCameraProxy;
sp<SurfaceComposerClient> mComposerClient;

int32_t mCameraId    = 0;
int32_t mFrameRate   = 30; //always 30, do not change
int mWidth  = 640 ;
int mHeight = 480 ;
Size videoSize;

void create_surface(){
    mComposerClient = new SurfaceComposerClient;
    mSurfaceControl = mComposerClient->createSurface(
                                        String8("Camera native Surface"),
                                        mWidth,
                                        mHeight,
                                        HAL_PIXEL_FORMAT_YCrCb_420_SP, /*PIXEL_FORMAT_RGBA_8888,*/
                                        0);
    if(!mSurfaceControl->isValid()){
        printf("mSurfaceControl not Valid\n");
    }
    SurfaceComposerClient::openGlobalTransaction();
    mSurfaceControl->setLayer(30000);
    mSurfaceControl->setPosition(150, 50);
    mSurfaceControl->setSize(mWidth,mHeight);
    mSurfaceControl->show();
    SurfaceComposerClient::closeGlobalTransaction();
    mPreviewSurface = mSurfaceControl->getSurface();
    if(mPreviewSurface != NULL){
	 gbp = mPreviewSurface->getIGraphicBufferProducer();
    }else {
	printf("mPreviewSurface NULL\n");
    }
}

int main(int argc, char **argv) {
    android::ProcessState::self()->startThreadPool();
    status_t err = OK;
    FILE *fp;

    videoSize.width = mWidth;
    videoSize.height = mHeight;
    
  //  if((fp = fopen("/mnt/sdcard/camera.h264", "a+")) == NULL){
    //    printf("open /mnt/sdcard/camera.h264 failed\n");
   // }
    //set surface for display camera video view
    create_surface();

    //using front camera
    //camera = Camera::connect(mCameraId);
    String16 clientName("CameraCPP", 9);
    camera = Camera::connect(mCameraId, clientName,Camera::USE_CALLING_UID);
    if(camera != NULL){
         android::String8 s = camera->getParameters();
         mCameraParams = new android::CameraParameters(s);
         mCameraParams->setVideoSize(videoSize.width, videoSize.height);
         mCameraParams->setPreviewSize(videoSize.width, videoSize.height);
         mCameraParams->set(CameraParameters::KEY_VIDEO_FRAME_FORMAT,CameraParameters::PIXEL_FORMAT_YUV420SP);
         mCameraParams->setPreviewFormat(CameraParameters::PIXEL_FORMAT_YUV420SP);
         mCameraParams->setPreviewFrameRate(mFrameRate);
         camera->setParameters(mCameraParams->flatten());
        // camera->setPreviewDisplay(mPreviewSurface);
	 camera->setPreviewTarget(gbp);
         camera->startPreview();
	 getchar();
	 camera->stopPreview();
	 camera->disconnect();
   }

    return 0;
}
