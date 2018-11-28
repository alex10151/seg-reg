#include"PreDef.h"
#include"Pipeline.h"
#include"RawReader.h"
using namespace std;
using namespace predef;

template<typename T>
vector<T> Parse2Vec(T*params)
{
	vector<T>temp;
	for (unsigned int i = 0; i < inputDim; i++)
		temp.push_back(params[i]);
	return temp;
}
int main(int argc, char* argv[])
{
	cout << "======================================" << endl;
	if (argc < 5)
	{
		if (!strcmp(argv[1],"-table") || !strcmp(argv[1],"-bodybone")||!strcmp(argv[1],"-headbone"))
		{
			cout << "missing params!" << endl;
			cout << "required parameters: -options[-table,-bodybone,-headbone] " << argv[1] << endl;
			cout << "required parameters: srcPath " << argv[2] << endl;
			cout << "required parameters:  dstPath" << argv[3] << endl;
			cout << "required parameters:  dstPathforBones" << argv[4] << endl;
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
	if (strcmp(argv[1],"-reg"))
	{
		cout << " parameters: -options[-table,-headbone,-bodybone] " << argv[1] << endl;
		cout << " parameters: srcPath " << argv[2] << endl;
		cout << " parameters:  dstPath" << argv[3] << endl;
		cout << " parameters:  dstPathforbones" << argv[4] << endl;
		unsigned int size[] = {168,168,206};
		float space[] = { 0.9765625 ,0.9765625 ,4};
		float origin[] = { 0,0,0 };
		int a = size[0] * size[1] * size[2];
		short *testdata = new short[a];
		short*b = new short[a];
		short*c = new short[a];
		for (unsigned int i = 0; i < a; i++)
			testdata[i] = i % (100 * 100);
		unsigned int testsize[3] = { 100,100,10 };
		RawReader<short, 3> raw1(Parse2Vec(size), Parse2Vec(space), Parse2Vec(origin), false, true);
		raw1.SetFilename(argv[2]);
		raw1.Run();
		RemovalPipelinev2(argv[1], testdata, b, c, size, space, origin);
		RemovalPipeline(argv[1], argv[2], argv[3],argv[4],size,space,origin,false,true);
		return EXIT_SUCCESS;
	}
	else
	{
		cout << " parameters: -options[-reg] " << argv[1] << endl;
		cout << " parameters: fixedPath " << argv[2] << endl;
		cout << " parameters:  movingPath" << argv[3] << endl;
		cout << " parameters: savePath" << argv[4] << endl;
		unsigned int size[] = { 512,512,206 };
		float space[] = { 0.9765625 ,0.9765625 ,4 };
		float origin[] = { 0,0,0 };
		unsigned int  size1[]= { 168,168,206 };
		float space1[] = { 2.90917 ,2.90917 ,4 };
		float origin1[] = { 0,0,0 };
		//short *testdata=new short[100*100*10];
		//for (unsigned int i = 0; i < 100 * 100 * 10; i++)
		//	testdata[i] = i%(100*100);
		//unsigned int testsize[3] = { 100,100,10 };
		//RawReader<short,3> raw1(Parse2Vec(size), Parse2Vec(space), Parse2Vec(origin),false,true);
		//raw1.SetFilename(argv[2]);
		//raw1.Run();
		////short*testdata = raw1.GetOutput()->GetBufferPointer();
		//short* res = RegistrationPipelinev2(testdata, testdata, testsize, space, origin, testsize, space, origin);
		RegistrationPipeline(argv[2], argv[3], argv[4],size,space,origin,false,true,size1,space1,origin1,false,true);
		return EXIT_SUCCESS;
	}
}