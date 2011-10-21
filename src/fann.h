#include <v8.h>
#include <node.h>
#include <doublefann.h>

using namespace v8;
using namespace node;

const char TRAIN_PREFIX[]  = "FANN_TRAIN_";
const int TRAIN_PREFIX_LEN = sizeof(TRAIN_PREFIX)-1;
 
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
		static void SetLearningRate(Local<String> property, Local<Value> value, const AccessorInfo& info);
		static void SetLearningMomentum(Local<String> property, Local<Value> value, const AccessorInfo& info);
		static Handle<Value> GetSmth(Local<String> property, const AccessorInfo &info) {
			Local<Object> self = info.Holder();
			NNet *net = ObjectWrap::Unwrap<NNet>(self);
			return Integer::New(net->something);
		};
		static void SetSmth(Local<String> property, Local<Value> value, const AccessorInfo& info) {
			Local<Object> self = info.Holder();
			NNet *net = ObjectWrap::Unwrap<NNet>(self);
			net->something = value->IntegerValue();
//			something = value->IntegerValue();
		};
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
		static Handle<Value> NormalizeAlgorithmName(const char* origname);
};
