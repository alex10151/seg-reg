#include"PreDef.h"
#include"Pipeline.h"
using namespace std;
using namespace predef;


int main(int argc, char* argv[])
{
	cout << "======================================" << endl;
	if (argc < 4)
	{
		if ((argv[1] == "-table") || (argv[1] == "-bodybone")||(argv[1]=="headbone"))
		{
			cout << "missing params!" << endl;
			cout << "required parameters: -options[-table,-bodybone,-headbone] " << argv[1] << endl;
			cout << "required parameters: srcPath " << argv[2] << endl;
			cout << "required parameters:  dstPath" << argv[3] << endl;
		}
		else
		{
			cout << "missing params!" << endl;
			cout << "required parameters: -options[-reg] " << argv[1] << endl;
			cout << "required parameters: fixedPath " << argv[2] << endl;
			cout << "required parameters:  movingPath" << argv[3] << endl;
			cout << "required parameters: savePath" << argv[4] << endl;
		}
		return EXIT_FAILURE;
	}
	if (argc == 4)
	{
		cout << " parameters: -options[-table,-headbone,-bodybone] " << argv[1] << endl;
		cout << " parameters: srcPath " << argv[2] << endl;
		cout << " parameters:  dstPath" << argv[3] << endl;
		RemovalPipeline(argv[1], argv[2], argv[3]);
		return EXIT_SUCCESS;
	}
	else
	{
		cout << " parameters: -options[-reg] " << argv[1] << endl;
		cout << " parameters: fixedPath " << argv[2] << endl;
		cout << " parameters:  movingPath" << argv[3] << endl;
		cout << " parameters: savePath" << argv[4] << endl;
		RegistrationPipeline(argv[2], argv[3], argv[4]);
		return EXIT_SUCCESS;
	}
}