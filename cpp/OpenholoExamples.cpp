#include "ophPointCloud.h"
#include "ophDepthMap.h"
#include "ophLightField.h"
#include "ophTriMesh.h"
#include "ophWRP.h"

#include "ophWaveAberration.h"
#include "ophCascadedPropagation.h"

#include "ophSig.h"
#include "ophSigPU.h"
#include "ophSigCH.h"

#define POINT_CLOUD		true			// Point Cloud
#define DEPTH_MAP		true			// Depth Map
#define LIGHT_FIELD		true			// Light Field
#define TRI_MESH		true			// Triangle Mesh
#define WRP				true			// WRP

#define ENCODE			true			// Encode

#define WAVE_ABERR		true			// Wave Aberration
#define CAS_PROPA		true			// Cascaded Propagation

#define OFF_AXIS		true			// Convert Off-axis
#define CONV_CAC		true			// Convert CAC
#define CONV_HPO		true			// Convert HPO
#define GET_AT			true			// Get Parameter AT
#define GET_SF			true			// Get Parameter SF
#define SIG_PSDH		true			// Signal Phase Shift
#define SIG_PU			true			// Signal Phase Unwrapping
#define SIG_CH			true			// Signal Compressive Holography

int main()
{
#if POINT_CLOUD & true
{
	cout << "OpenHolo Library : Generation Hologram - Point Cloud Example" << endl;

	ophPointCloud* Hologram = new ophPointCloud();

	Hologram->readConfig("config/TestSpecPointCloud.xml");						// Read Config Parameters for Point Cloud CGH
	Hologram->loadPointCloud("source/PointCloud/TestPointCloud_Plane.ply");		// Load Point Cloud Data(*.PLY)

	Hologram->setMode(MODE_GPU);												// Select CPU or GPU Processing

	Hologram->generateHologram(PC_DIFF_RS);										// CGH by R-S Diffract
	Hologram->saveAsOhc("result/PointCloud/Result_PointCloudSample_Plane");		// Save Hologram Complex Field by *.OHC

	Hologram->encodeHologram();													// Encode Complex Field to Real Field
	Hologram->normalize();														// Normalize Real Field to unsigned char(0~255) for save to image(*.BMP)

	Hologram->save("result/PointCloud/Result_PointCloudSample_Plane.bmp");		// Save to bmp

	//Hologram->loadAsOhc("result/PointCloud/Result_PointCloudSample_Plane");	// Load Complex Field by *.OHC

	//Hologram->setMode(MODE_GPU);												// Select CPU or GPU Processing

	//Hologram->setOffsetDepth(0.3);											// Set Point Cloud Config Parameter - Offset Depth
	//Hologram->setScale(0.01, 0.01, 0.01);										// Set Point Cloud Config Parameter - Scale of Model Data

	//Hologram->encodeHologram();												// Encode Complex Field to Real Field
	//Hologram->normalize();													// Normalize Real Field to unsigned char(0~255) for save to image(*.BMP)

	//Hologram->save("result/PointCloud/Result_PointCloudSample_PlaneByOHC");	// Save to bmp

	Hologram->release();														// Release memory used to Generate Point Cloud
}
#endif
#if DEPTH_MAP & true
{
	cout << "OpenHolo Library : Generation Hologram - Depth Map Example" << endl;

	ophDepthMap* Hologram = new ophDepthMap();

	Hologram->setMode(MODE_GPU);												// Select CPU or GPU Processing
	Hologram->readConfig("config/TestSpecDepthMap.xml");						// Read Config Parameters for Depth Map CGH
	Hologram->readImageDepth("source/DepthMap", "RGB_D", "D_D");				// Read depth image & rgb image

	Hologram->generateHologram();												// CGH by depth map
	Hologram->encodeHologram();													// Encode Complex Field to Real Field
	Hologram->normalize();														// Normalize Real Field to unsigned char(0~255) for save to image(*.BMP)
	Hologram->save("result/DepthMap/Result_DepthmapSample.bmp");				// Save to bmp

	Hologram->release();														// Release memory used to Generate Depth Map
}
#endif
#if LIGHT_FIELD & true
{
	ophLF* Hologram = new ophLF();

	// Load
	Hologram->readLFConfig("config/TestSpecLF.xml");							// Read the LF hologram configuration file
	Hologram->loadLF("source/LightField", "bmp");// Load the Light field source image files
		/// Put the directory which has the source files and Put the image file type

	// Generate
	Hologram->generateHologram();												// Generate the hologram

	// Save as Complex field data
	Hologram->saveAsOhc("result/LightField/LF_complexField.ohc");				// Save the hologram complex field data

	// Encode
	Hologram->encoding(Hologram->ENCODE_SIMPLENI);								// Encode the hologram

	// Save as Encoded Image
	Hologram->normalizeEncoded();												// Normalize the encoded hologram to generate image file
	ivec2 encode_size = Hologram->getEncodeSize();								// Get encoded hologram size
	Hologram->save("result/LightField/Light_Field_NI_carrier.bmp",
		8, nullptr, encode_size[_X], encode_size[_Y]);							// Save the encoded hologram image

	Hologram->release();														// Release memory used to Generate Light Field
}
#endif
#if TRI_MESH & true
{
	ophTri* Hologram = new ophTri();

	// Load
	Hologram->readMeshConfig("config/TestSpecMesh.xml");						// Read the Mesh hologram configuration file
	Hologram->loadMeshData("source/TriMesh/mesh_teapot.ply", "ply");			// Read the Meshed object data
	Hologram->objScaleShift();													// Object scaling and shifting

	// Generate
	Hologram->generateMeshHologram(Hologram->SHADING_FLAT);						// Generate the hologram
		/// Put the shading effect type

	// Save as Complex Field Data
	Hologram->saveAsOhc("result/TriMesh/Mesh_complexField.ohc");				// Save the hologram complex field data

	// Encode
	Hologram->encoding(Hologram->ENCODE_SIMPLENI);								// Encode the hologram

	// Save as Encoded Image
	Hologram->normalizeEncoded();												// Normalize the encoded hologram to generate image file
	ivec2 encode_size = Hologram->getEncodeSize();								// Get encoded hologram size
	Hologram->save("result/TriMesh/Mesh_0.1m_ni_-0.3deg.bmp",
		8, nullptr, encode_size[_X], encode_size[_Y]);							// Save the encoded hologram image

	Hologram->release();														// Release memory used to Generate Triangle Mesh
}
#endif
#if WRP & true
{
	cout << "OpenHolo Library : Generation Hologram - Wavefront Recording Plane(WRP) Example" << endl;

	ophWRP* Hologram = new ophWRP();                                   // ophWRP instance

	Hologram->readConfig("config/TestSpecWRP.xml");                    // Read Config Parameters for Point Cloud CGH based WRP algorism
	Hologram->loadPointCloud("source/WRP/TestPointCloud_WRP.ply");     // Load Point Cloud Data(*.PLY)


	Hologram->calculateWRP();                                          //  WRP generation 
	Hologram->generateHologram();                                      // CGH from WRP
	Hologram->encodeHologram();                                        // Encode Complex Field to Real Field
	Hologram->normalize();                                             //Normalize Real Field to unsigned char(0~255) for save to image(*.BMP)
	Hologram->save("result/WRP/Result_WRP.bmp");                       // Save to bmp

	Hologram->release();                                               // Release memory used to Generate Point Cloud 
}
#endif
#if ENCODE & true
{
	ophPointCloud* Hologram = new ophPointCloud();

	Hologram->loadComplex("source/Encoding/teapot_real_1920,1080.txt", "source/Encoding/teapot_imag_1920,1080.txt", 1920, 1080);

	Hologram->encoding(ophGen::ENCODE_AMPLITUDE);

	Hologram->normalizeEncoded();

	ivec2 encode_size = Hologram->getEncodeSize();

	Hologram->save("result/Encoding/Encoding.bmp", 8, nullptr, encode_size[_X], encode_size[_Y]);

	Hologram->release();
}
#endif
#if WAVE_ABERR & true
{
	ophWaveAberration* wa = new ophWaveAberration;

	wa->readConfig("config/TestSpecAberration.xml");  // reads parameters from a configuration file
	wa->accumulateZernikePolynomial();         // generates 2D complex data array of wave aberration according to parameters   
	wa->complex_W;                            // double pointer variable of 2D complex data array of wave aberration
	wa->resolutionX;                           // resolution in x axis of 2D complex data array of wave aberration
	wa->resolutionY;                           // resolution in y axis of 2D complex data array of wave aberration
	wa->saveAberration("result/WaveAberration/aberration.bin");      // saves 2D complex data array of complex wave aberration into a file

	wa->readAberration("result/WaveAberration/aberration.bin");      // reads 2D complex data array of complex wave aberration from a file
	wa->complex_W;                            // double pointer variable of 2D complex data array of wave aberration
	wa->resolutionX;                           // resolution in x axis of 2D complex data array of wave aberration
	wa->resolutionY;                           // resolution in y axis of 2D complex data array of wave aberration


	wa->release();
}
#endif
#if CAS_PROPA & true
{
	ophCascadedPropagation* pCp = new ophCascadedPropagation(L"config/TestSpecCascadedPropagation.xml");	// ophCascadedPropagation instance generation and parameter setup
	if (pCp->isReadyToPropagate()																			// check if all the input are ready
		&& pCp->propagateSlmToPupil()																		// 1st propagation: from SLM to pupil
		&& pCp->propagatePupilToRetina())																	// 2nd propagation: from pupil to retina
		pCp->save(L"result/CascadedPropagation/intensityRGB.bmp", pCp->getNumColors() * 8);					// save numerical reconstruction result in BMP format

	pCp->release();
}
#endif
#if OFF_AXIS & true
{		
	std::cout << "OpenHolo Library : Hologram core processing - Off-axis hologram transform Example" << std::endl;

	//declaration ophSig class
	ophSig *holo = new ophSig();

	//read parameter
	if (!holo->readConfig("config/holoParam.xml")) {
		// no file 
		return false;
	}

	//hologram data load
	if (!holo->load("source/OffAxis/3_point_re.bmp", "source/OffAxis/3_point_im.bmp", 8)) {
		// no file 
		return false;
	}

	//run Convert Offaxis function
	holo->sigConvertOffaxis();

	//save hologram data for bmp file
	holo->save("result/OffAxis/Off_axis.bmp", 8);

	//release
	holo->release();
}
#endif
#if CONV_CAC & true
{
	std::cout << "OpenHolo Library : Hologram core processing - CAC transform Example" << std::endl;

	//declaration ophSig class
	ophSig *holo = new ophSig();

	//read parameter
	if (!holo->readConfig("config/holoParam.xml")) {
		// no file 
		return false;
	}

	//hologram data load
	if (!holo->load("source/CAC/ColorPoint_re.bmp", "source/CAC/ColorPoint_im.bmp", 24)) {
		// no file 
		return false;
	}
		
	//run convert chromatic aberration compensation
	holo->sigConvertCAC(0.000000633, 0.000000532, 0.000000473);

	//save hologram data for bmp file
	holo->save("result/CAC/CAC_re_C.bin", "result/CAC/CAC_im_C.bin", 24);

	//release
	holo->release();
}
#endif
#if CONV_HPO & true
{
	std::cout << "OpenHolo Library : Hologram core processing - HPO transform Example" << std::endl;

	//declaration ophSig class
	ophSig *holo = new ophSig();

	//read parameter
	if (!holo->readConfig("config/holoParam.xml")) {
		// no file 
		return false;
	}

	//hologram data load
	if (!holo->load("source/HPO/3_point_re.bmp", "source/HPO/3_point_im.bmp", 8)) {
		// no file 
		return false;
	}

	//run convert horizontal parallax only hologram
	holo->sigConvertHPO();

	//save hologram data for bmp file
	holo->save("result/HPO/HPO_re.bmp", "result/HPO/HPO_im.bmp", 8);

	//release
	holo->release();
}
#endif
#if GET_AT & true
{
std::cout << "OpenHolo Library : Hologram core processing - get parameter using axis transformation Example" << std::endl;

ophSig* holo = new ophSig();

float depth = 0;

if (!holo->readConfig("config/holoParam.xml")) {
	// no file 
	return false;
}

//hologram data load
if (!holo->load("source/AT/0.1point_re.bmp", "source/AT/0.1point_im.bmp", 8)) {
	// no file 
	return false;
}

//get parameter using axis transformation
depth = holo->sigGetParamAT();

std::cout << depth << endl;

// backpropagation
holo->propagationHolo(-depth);

//save hologram data for bmp file
holo->save("result/AT/AT_re.bmp", "result/AT/AT_im.bmp", 8);

	//release
	holo->release();
}
#endif
#if GET_SF & true
{
	std::cout << "OpenHolo Library : Hologram core processing - get parameter using axis transformation Example" << std::endl;

	////declaration ophSig class
	ophSig* holo = new ophSig();

	float depth = 0;

	//read parameter
	if (!holo->readConfig("config/holoParam.xml")) {
		// no file 
		return false;
	}

	//hologram data load
	if (!holo->load("source/SF/3_point_re.bmp", "source/SF/3_point_im.bmp", 8)) {
		// no file 
		return false;
	}

	//get parameter using sharpness function
	depth = holo->sigGetParamSF(10, -10, 100, 0.3);
	std::cout << depth << endl;

	// backpropagation
	holo->propagationHolo(depth);

	//save hologram data for bmp file
	holo->save("result/SF/SF_re.bmp", "result/SF/SF_im.bmp", 8);

	//release
	holo->release();
}
#endif
#if SIG_PSDH & true
{
	std::cout << "OpenHolo Library : Hologram core processing - Complex Field from Phase Shift Digital Hologram Example" << std::endl;

	ophSig *holo = new ophSig();

	const char *f0 = "source/PhaseShiftedHolograms/0930_005_gray.bmp";		// File names of interference patterns with 90 degree reference wave phase shifts
	const char *f90 = "source/PhaseShiftedHolograms/0930_006_gray.bmp";
	const char *f180 = "source/PhaseShiftedHolograms/0930_007_gray.bmp";
	const char *f270 = "source/PhaseShiftedHolograms/0930_008_gray.bmp";

	holo->getComplexHFromPSDH(f0, f90, f180, f270);							// Extract complex field from 4 interference patterns

	holo->save("result/PhaseShift/PSDH_re_C.bmp", "result/PhaseShift/PSDH_im_C.bmp", 8); // Save complex field to image files (real and imaginary parts)
	holo->release();
}
#endif
#if SIG_PU & true
{
	std::cout << "OpenHolo Library : Hologram core processing - Phase Unwrapping Example" << std::endl;

	ophSigPU *holo = new ophSigPU;

	// Load original wrapped phase data
	if (!holo->loadPhaseOriginal("source/PhaseUnwrapping/phase_unwrapping_example.bmp", 8)) {
		return false;
	}
	int maxBoxRadius = 4;
	holo->setPUparam(maxBoxRadius);		// Set phase unwrapping algorithm parameter (maximum box radius for Goldstein phase unwrapping)

	// Do phase unwrapping algorithm
	holo->runPU();

	// Save results
	holo->savePhaseUnwrapped("result/PhaseUnwrapping/PU_Test.bmp");
	holo->release();
}
#endif
#if SIG_CH & true
{
	std::cout << "OpenHolo Library : Hologram core processing - Compressed Holography Example" << std::endl;

	ophSigCH *holo = new ophSigCH;

	// Load configure file (xml)
	if (!holo->readConfig("config/TestSpecCH.xml")) {
		return false;
	}

	if (!holo->loadCHtemp("source/CompressiveHolo/sampleComplexH_re.bmp", "source/CompressiveHolo/sampleComplexH_im.bmp", 8)) {
		return false;
	}

	// Do compressed Hologram algorithm
	holo->runCH(0);

	// Save results (numerical reconstructions) to image files
	holo->saveNumRec("result/CompressiveHolo/CH_Test.bmp");
	holo->release();
}
#endif



return 0;
}