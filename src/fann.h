#include <v8.h>
#include <node.h>
#include <doublefann.h>

using namespace v8;
using namespace node;

const char TRAIN_PREFIX[]  = "FANN_TRAIN_";
const char FANN_PREFIX[]  = "FANN_";
const int TRAIN_PREFIX_LEN = sizeof(TRAIN_PREFIX)-1;
const int FANN_PREFIX_LEN = sizeof(FANN_PREFIX)-1;
 
class NNet : public ObjectWrap
{
	public:
		int something;
		NNet();
		~NNet();
		Handle<Value> CreateStandard(const Arguments &args);
		Handle<Value> CreateSparse(const Arguments &args);
		Handle<Value> CreateShortcut(const Arguments &args);
		//Handle<Value> CreateClone(const Arguments &args);
		static void Initialize(Handle<Object> target);
		static Handle<Value> NewStandard(const Arguments &args);
		static Handle<Value> NewSparse(const Arguments &args);
		static Handle<Value> NewShortcut(const Arguments &args);
		//static Handle<Value> CloneNet(const Arguments &args);
		static Handle<Value> GetTrainingAlgorithm(Local<String> property, const AccessorInfo &info);
		static void SetTrainingAlgorithm(Local<String> property, Local<Value> value, const AccessorInfo& info);
		static Handle<Value> GetTrainingAlgorithmList(const Arguments &args);
		static Handle<Value> GetLearningRate(Local<String> property, const AccessorInfo &info);
		static Handle<Value> GetLearningMomentum(Local<String> property, const AccessorInfo &info);
		static Handle<Value> ActivationFunction(const Arguments &args);
		static Handle<Value> ActivationFunctionHidden(const Arguments &args);
		static Handle<Value> ActivationFunctionOutput(const Arguments &args);
		static Handle<Value> GetActivationFunctionList(const Arguments &args);
		static void SetLearningRate(Local<String> property, Local<Value> value, const AccessorInfo& info);
		static void SetLearningMomentum(Local<String> property, Local<Value> value, const AccessorInfo& info);
		static Handle<Value> Train(const Arguments &args);
		static Handle<Value> TrainOnce(const Arguments &args);
		static Handle<Value> Run(const Arguments &args);
	private:
		struct fann *FANN;
		bool scale_present;
		int _GetLayersFromArray(unsigned int *&layers, Local<Array> a);
		int _GetLayersFromArgs(unsigned int *&layers, const Arguments *args, int skip=0);
		static void PrototypeInit(Local<FunctionTemplate> t);
		Handle<Value> MakeTrainData(const Arguments &args, struct fann_train_data **traindata);
		Handle<Value> TrainOnData(struct fann_train_data *traindata, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error);
		static Handle<Value> NormalizeName(const char* origname, const char* prefix, int prefix_len);
		static int _SeekCharArray(Local<String> value, const char* const* array, int size, const char* prefix);		
};

Handle<Value> VException(const char *msg);

