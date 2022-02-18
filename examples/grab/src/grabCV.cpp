#include <pylon/PylonIncludes.h>
#include <opencv4/opencv2/opencv.hpp>

using namespace Pylon;
using namespace std;

using namespace GENAPI_NAMESPACE;

// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 100;

int main( int /*argc*/, char* /*argv*/[] ) {
    int exitCode = 0;
    PylonInitialize();

    try {
        // Create an instant camera object with the camera device found first.
        CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice() );

        // Print the model name of the camera.
        cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

        camera.MaxNumBuffer = 10;
        camera.Open();

        // Change camera settings through GENAPI
        INodeMap& nodemap = camera.GetNodeMap();

        CIntegerPtr width = nodemap.GetNode("Width");
        CIntegerPtr height = nodemap.GetNode("Height");

        CImageFormatConverter formatConverter;
        formatConverter.OutputPixelFormat = PixelType_BGR8packed;

        CPylonImage pylonImage;
        cv::Mat openCvImage;

        camera.StartGrabbing( c_countOfImagesToGrab );
        CGrabResultPtr ptrGrabResult;

        while (camera.IsGrabbing()) {
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException );

            if (ptrGrabResult->GrabSucceeded()) {
                // Access the image data.
                cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
                cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
                const uint8_t* pImageBuffer = (uint8_t*) ptrGrabResult->GetBuffer();
                cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;

                formatConverter.Convert(pylonImage, ptrGrabResult);

                openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t * ) pylonImage.GetBuffer());

                cv::imshow("pylon", openCvImage);
                cv::waitKey(1);
            } else {
                cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " " << ptrGrabResult->GetErrorDescription() << endl;
            }
        }
    } catch (const GenericException& e) {
        cerr << "An exception occurred." << endl << e.GetDescription() << endl;
        exitCode = 1;
    }

    cerr << endl << "Press enter to exit." << endl;
    while (cin.get() != '\n');
    
    PylonTerminate();

    return exitCode;
}