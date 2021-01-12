<<<<<<< HEAD
/**
* @file MeteorData.h
*
* Classes for application data
*
*/

#ifndef _METEORDATA_H_
#define _METEORDATA_H_



namespace FindMeteor
{
	/**
	*	@brief Class for the application data
	*/
	class CMeteorData
	{
	public:

		CMeteorData(void) { Cleanup(); }
		~CMeteorData(void) { Cleanup(); }

		/// clean up data
		void Cleanup(void)
		{
			m_isTesting = false;
			m_needOutput = false;
			m_doEvaluation = false;
		}

	
	public:
		
		/*  Preparation  */ 
		bool m_isTesting;								// �O�_�b����
		bool m_needOutput;							// �O�_�n��X�p��L�{���Ϥ�
		bool m_doEvaluation;							// �O�_�n��C�ӨB�J�PGT�i����R�ÿ�X���G


		// Path
		std::string srcFilePath;								// �Ϥ����|
		std::string dataSubPath;								// �Ϥ����|
		std::string srcFileName;								// �Ϥ��W�١]���]�t���A�^
		std::string srcFileNameT;							// �Ϥ��W�١]�]�t���A�^
		std::string filename;								// json�ɦs���m�]�P�Ϥ��ɸ��|�^
		std::string outputFolderPath;

		// Image
		cv::Mat srcLoadImg;									// �x�s��J���Ϥ�


	};
}

=======
/**
* @file MeteorData.h
*
* Classes for application data
*
*/

#ifndef _METEORDATA_H_
#define _METEORDATA_H_



namespace FindMeteor
{
	/**
	*	@brief Class for the application data
	*/
	class CMeteorData
	{
	public:

		CMeteorData(void) { Cleanup(); }
		~CMeteorData(void) { Cleanup(); }

		/// clean up data
		void Cleanup(void)
		{
			m_isTesting = false;
			m_needOutput = false;
			m_doEvaluation = false;
		}

	
	public:
		
		/*  Preparation  */ 
		bool m_isTesting;								// �O�_�b����
		bool m_needOutput;							// �O�_�n��X�p��L�{���Ϥ�
		bool m_doEvaluation;							// �O�_�n��C�ӨB�J�PGT�i����R�ÿ�X���G


		// Path
		std::string srcFilePath;								// �Ϥ����|
		std::string dataSubPath;								// �Ϥ����|
		std::string srcFileName;								// �Ϥ��W�١]���]�t���A�^
		std::string srcFileNameT;							// �Ϥ��W�١]�]�t���A�^
		std::string filename;								// json�ɦs���m�]�P�Ϥ��ɸ��|�^
		std::string outputFolderPath;

		// Image
		cv::Mat srcLoadImg;									// �x�s��J���Ϥ�


	};
}

>>>>>>> CppVersion
#endif