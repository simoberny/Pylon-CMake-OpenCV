#include <pylon/PylonIncludes.h>
#include <opencv4/opencv2/opencv.hpp>

#include "../include/ConfigurationEventPrinter.h"
#include "../include/ImageEventPrinter.h"

#include <thread> 

using namespace Pylon;
using namespace std;
using namespace GENAPI_NAMESPACE;

class CGetImage : public CImageEventHandler {
    public:
        virtual void OnImageGrabbed( CInstantCamera& /*camera*/, const CGrabResultPtr& ptrGrabResult ) {
            CImageFormatConverter formatConverter;
            formatConverter.OutputPixelFormat = PixelType_BGR8packed;

            if(ptrGrabResult->GrabSucceeded()){
                CPylonImage pylonImage;
                formatConverter.Convert(pylonImage, ptrGrabResult);

                cv::Mat openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t * ) pylonImage.GetBuffer());

                cv::imshow("pylon", openCvImage);
                cv::waitKey(1);
            }

            cout << "Image number: " << ptrGrabResult->GetImageNumber() << endl;
        }
};

void runScheduler(CInstantCamera * camera){
    while (true){
        if(camera->WaitForFrameTriggerReady(1000, TimeoutHandling_ThrowException)){
            camera->ExecuteSoftwareTrigger();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

int main( int /*argc*/, char* /*argv*/[] )
{
    int exitCode = 0;
    PylonInitialize();

    try
    {
        CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice() );

        // Register the standard configuration event handler for enabling software triggering.
        camera.RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete );

        //camera.RegisterConfiguration( new CConfigurationEventPrinter, RegistrationMode_Append, Cleanup_Delete );
        camera.RegisterImageEventHandler( new CImageEventPrinter, RegistrationMode_Append, Cleanup_Delete );
        camera.RegisterImageEventHandler( new CGetImage, RegistrationMode_Append, Cleanup_Delete );

        camera.Open();

        camera.StartGrabbing( GrabStrategy_LatestImageOnly, GrabLoop_ProvidedByInstantCamera);

        std::thread schedThread (runScheduler, &camera);

        cerr << endl << "Press enter to exit." << endl;
        while (cin.get() != '\n');

        schedThread.join();
    } catch (const GenericException& e){
        cerr << "An exception occurred." << endl << e.GetDescription() << endl;
        exitCode = 1;
    }

    PylonTerminate();

    return exitCode;
}
