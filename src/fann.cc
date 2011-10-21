#include <v8.h>
#include <node.h>
#include <doublefann.h>
#include <string.h>
#include "fann.h"

using namespace v8;
using namespace node;

NNet::NNet()
{
	FANN = NULL;
	scale_present = false;
}

NNet::~NNet()
{
	if (FANN != NULL) {
		fann_destroy(FANN);
		FANN = NULL;
	}
}

Handle<Value> VException(const char *msg)
{
	HandleScope scope;
	return ThrowException(Exception::Error(String::New(msg)));
}

int NNet::_GetLayersFromArray(unsigned int *&layers, Local<Array> a)
{
    HandleScope scope;
	int len = a->Length();
	if (len < 2)
		return 0;

	layers = new unsigned int[len];
	for (int i=0; i<a->Length(); i++) {
		int n = a->Get(i)->IntegerValue();
		if (n < 1)
			return 0;
		layers[i] = n;
	}
	return len;
}

int NNet::_GetLayersFromArgs(unsigned int *&layers, const Arguments *args, int skip)
{
    HandleScope scope;
	int len = args->Length();
	if (len < 2)
		return 0;

	layers = new unsigned int[len];
	for (int i=skip; i<args->Length(); i++) {
		int n = (*args)[i]->IntegerValue();
		if (n < 1)
			return 0;
		layers[i] = n;
	}
	return len;
}

Handle<Value> NNet::NewStandard(const Arguments &args)
{
    HandleScope scope;
	NNet *net = new NNet();
	net->Wrap(args.This());
	net->CreateStandard(args);
    return args.This();
}

Handle<Value> NNet::NewSparse(const Arguments &args)
{
    HandleScope scope;
	NNet *net = new NNet();
	net->Wrap(args.This());
	net->CreateSparse(args);
    return args.This();
}

Handle<Value> NNet::NewShortcut(const Arguments &args)
{
    HandleScope scope;
	NNet *net = new NNet();
	net->Wrap(args.This());
	net->CreateShortcut(args);
    return args.This();
}

/* for FANN >= 2.2.0
Handle<Value> NNet::CloneNet(const Arguments &args)
{
    HandleScope scope;
//    if (!NNet::HasInstance(args[0]))
//        return VException("First argument must be existing network.");
	NNet *net = new NNet();
	net->Wrap(args.This());
	net->CreateClone(args);
    return args.This();
}*/

Handle<Value> NNet::CreateStandard(const Arguments &args)
{
	unsigned int* layers = NULL;
	int len = 0;
    
	if (args.Length() < 1)
        return VException("No arguments supplied");

	if (args[0]->IsArray()) {
		len = _GetLayersFromArray(layers, Array::Cast(*args[0]->ToObject()));
	} else {
		len = _GetLayersFromArgs(layers, &args);
	}
	if (len <= 0) {
		if (layers != NULL) delete[] layers;
        return VException("Wrong arguments supplied");
	}

	FANN = fann_create_standard_array(len, layers);

/*const float desired_error = (const float) 0.001;
const unsigned int max_epochs = 500000;
const unsigned int epochs_between_reports = 1000;
fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

fann_train_on_file(ann, "xor.data", max_epochs, epochs_between_reports, desired_error);

fann_save(ann, "xor_float.net");

fann_destroy(ann);*/
	delete[] layers;
	return Undefined();
}

Handle<Value> NNet::CreateSparse(const Arguments &args)
{
	unsigned int* layers = NULL;
	int len = 0;
    
	if (args.Length() < 1)
        return VException("No arguments supplied");
	
	if (!args[0]->IsNumber())
        return VException("First argument should be float");

	if (args[1]->IsArray()) {
		len = _GetLayersFromArray(layers, Array::Cast(*args[1]->ToObject()));
	} else {
		len = _GetLayersFromArgs(layers, &args, 1);
	}
	if (len <= 0) {
		if (layers != NULL) delete[] layers;
        return VException("Wrong arguments supplied");
	}

	FANN = fann_create_sparse_array(args[0]->NumberValue(), len, layers);
	delete[] layers;
	return Undefined();
}

Handle<Value> NNet::CreateShortcut(const Arguments &args)
{
	unsigned int* layers = NULL;
	int len = 0;
    
	if (args.Length() < 1)
        return VException("No arguments supplied");

	if (args[0]->IsArray()) {
		len = _GetLayersFromArray(layers, Array::Cast(*args[0]->ToObject()));
	} else {
		len = _GetLayersFromArgs(layers, &args);
	}
	if (len <= 0) {
		if (layers != NULL) delete[] layers;
        return VException("Wrong arguments supplied");
	}

	FANN = fann_create_shortcut_array(len, layers);
	delete[] layers;
	return Undefined();
}

Handle<Value> NNet::NormalizeName(const char* origname, const char* prefix, int prefix_len)
{
	HandleScope scope;
	char algname[64];
	if (strncmp(origname, prefix, prefix_len) == 0) {
		origname +=	prefix_len;
	} 
	strncpy(algname, origname, 63);
	return scope.Close(String::New(origname));
}

Handle<Value> NNet::GetTrainingAlgorithm(Local<String> property, const AccessorInfo &info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);
	enum fann_train_enum algo = fann_get_training_algorithm(net->FANN);

	if (algo >= 0 && algo < size) {
		return scope.Close(NormalizeName(FANN_TRAIN_NAMES[algo], TRAIN_PREFIX, TRAIN_PREFIX_LEN));
	} else {
		return Undefined();
	}
}

int NNet::_SeekCharArray(Local<String> value, const char* const* array, int size, const char* prefix)
{
	char name[64];
	char name2[128];
	value->WriteAscii(name, 0, 63);
	strcpy(name2, prefix);
	strcat(name2, name);
	for (int i=0; i<size; i++) {
		if (strcasecmp(name, array[i]) == 0 || strcasecmp(name2, array[i]) == 0) {
			return i;
		}
	}
	return -1;
}

void NNet::SetTrainingAlgorithm(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);
	int num = -1;

	if (value->IsString()) {
		num = _SeekCharArray(String::Cast(*value), FANN_TRAIN_NAMES, size, TRAIN_PREFIX);
	} else if (value->IsNumber()) {
		num = value->NumberValue();
	}

	if (num >= 0 && num < size) {
		fann_set_training_algorithm(net->FANN, fann_train_enum(num));
	}
}

Handle<Value> NNet::GetTrainingAlgorithmList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_TRAIN_NAMES[i], TRAIN_PREFIX, TRAIN_PREFIX_LEN));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> NNet::GetActivationFunctionList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_ACTIVATIONFUNC_NAMES[i], FANN_PREFIX, FANN_PREFIX_LEN));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> NNet::GetLearningRate(Local<String> property, const AccessorInfo &info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	
	float rate = fann_get_learning_rate(net->FANN);
	return scope.Close(Number::New(rate));
}

Handle<Value> NNet::GetLearningMomentum(Local<String> property, const AccessorInfo &info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	
	float rate = fann_get_learning_momentum(net->FANN);
	return scope.Close(Number::New(rate));
}

void NNet::SetLearningRate(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);

	fann_set_learning_rate(net->FANN, value->NumberValue());
}

void NNet::SetLearningMomentum(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);

	fann_set_learning_momentum(net->FANN, value->NumberValue());
}

/* for FANN >= 2.2.0
Handle<Value> NNet::CreateClone(const Arguments &args)
{
	NNet *currnet = ObjectWrap::Unwrap<NNet>(args.This());
	NNet *oldnet = ObjectWrap::Unwrap<NNet>(args[0]->ToObject());
	currnet->FANN = fann_copy(oldnet->FANN);
//	printf("!!!!!!!!!!!!!! %d %d\n", currnet->something, oldnet->something);
	return Undefined();
}*/

fann_type *dataset_raw;
void FANN_API _TrainCB_FromRAW(unsigned int num, unsigned int innum, unsigned int outnum, fann_type *in, fann_type *out) {
	for (int c=0; c<innum; c++) {
		in[c] = dataset_raw[num*(innum+outnum) + c];
	}
	for (int c=0; c<outnum; c++) {
		out[c] = dataset_raw[num*(innum+outnum) + innum + c];
	}
}

Handle<Value> NNet::MakeTrainData(const Arguments &args, struct fann_train_data **traindata)
{
	if (args.Length() < 1)
        return VException("No arguments supplied");

	if (!args[0]->IsArray())
        return VException("First argument should be 2d-array (training data set)");
		
	int input_num = fann_get_num_input(FANN);
	int output_num = fann_get_num_output(FANN);
	
	Local<Array> dataset = Array::Cast(*args[0]->ToObject());
	dataset_raw = new fann_type[(input_num+output_num) * dataset->Length()];
	fann_type *dataset_raw_ptr = dataset_raw;
	//fann_set_user_data(net->FANN, (void*)dataset_raw);
	for (int i=0; i<dataset->Length(); i++) {
		Local<Value> t = dataset->Get(i);
		if (!t->IsArray()) {
			return VException("First argument should be 2d-array (training data set)");
		}
		Local<Array> data = Array::Cast(*t);
		if (data->Length() != 2) {
			return VException("Incorrect dataset");
		}
		Local<Value> tin = data->Get(0);
		Local<Value> tout = data->Get(1);
		if (!tin->IsArray() || !tout->IsArray()) {
			return VException("Incorrect dataset");
		}

		Local<Array> datain = Array::Cast(*tin);
		Local<Array> dataout = Array::Cast(*tout);
		if (datain->Length() != input_num) {
			return VException("Incorrect dataset (input length doesn't match input layer)");
		}
		if (dataout->Length() != output_num) {
			return VException("Incorrect dataset (output length doesn't match output layer)");
		}
		for (int c=0; c<input_num; c++) {
			*dataset_raw_ptr = datain->Get(c)->NumberValue();
			dataset_raw_ptr++;
		}
		for (int c=0; c<output_num; c++) {
			*dataset_raw_ptr = dataout->Get(c)->NumberValue();
			dataset_raw_ptr++;
		}
	}

	*traindata = fann_create_train_from_callback(dataset->Length(), input_num, output_num, _TrainCB_FromRAW);
	delete[] dataset_raw;
	
	return Undefined();
}

Handle<Value> NNet::TrainOnData(struct fann_train_data *traindata, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error)
{
	fann_train_on_data(FANN, traindata, max_epochs, epochs_between_reports, desired_error);
	return Undefined();
}

Handle<Value> NNet::Train(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	struct fann_train_data *traindata = NULL;
	net->MakeTrainData(args, &traindata);
	if (traindata == NULL)
        return VException("Internal error");

	unsigned int max_epochs = 100000;
	unsigned int epochs_between_reports = 1000;
	float desired_error = 0.001;
	int scale = 0;
	if (args.Length() >= 2) {
		Local<Object> params = *args[1]->ToObject();
		if (params->Has(String::New("epochs"))) {
			max_epochs = params->Get(String::New("epochs"))->IntegerValue();
		}
		if (params->Has(String::New("epochs_between_reports"))) {
			epochs_between_reports = params->Get(String::New("epochs_between_reports"))->IntegerValue();
		}
		if (params->Has(String::New("error"))) {
			desired_error = params->Get(String::New("error"))->NumberValue();
		}
		if (params->Has(String::New("scale"))) {
			scale = params->Get(String::New("scale"))->BooleanValue();
		}
	}
	if (scale) {
		fann_scale_train(net->FANN, traindata);
		net->scale_present = true;
	}
	net->TrainOnData(traindata, max_epochs, epochs_between_reports, desired_error);
	fann_destroy_train(traindata);
	return Undefined();
}

Handle<Value> NNet::TrainOnce(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	if (args.Length() < 2)
        return VException("2 arguments needed");

	if (!args[0]->IsArray())
        return VException("First argument should be array (training input)");
	if (!args[1]->IsArray())
        return VException("Second argument should be array (training output)");
	
	Local<Array> datain = Array::Cast(*args[0]->ToObject());
	Local<Array> dataout = Array::Cast(*args[1]->ToObject());
	fann_type *dataset_in = new fann_type[datain->Length()];
	fann_type *dataset_out = new fann_type[dataout->Length()];
	for (int i=0; i<datain->Length(); i++) {
		dataset_in[i] = datain->Get(i)->NumberValue();
	}
	for (int i=0; i<dataout->Length(); i++) {
		dataset_out[i] = dataout->Get(i)->NumberValue();
	}

	fann_train(net->FANN, dataset_in, dataset_out);

	delete[] dataset_in;
	delete[] dataset_out;
	return Undefined();
}

Handle<Value> NNet::Run(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	if (args.Length() < 1)
        return VException("No arguments supplied");
	if (!args[0]->IsArray())
        return VException("First argument should be array");

	Local<Array> datain = Array::Cast(*args[0]->ToObject());
	fann_type *dataset_in = new fann_type[datain->Length()];
	for (int i=0; i<datain->Length(); i++) {
		dataset_in[i] = datain->Get(i)->NumberValue();
	}

	fann_type *result = fann_run(net->FANN, dataset_in);
		
	if (net->scale_present) {
		fann_descale_output(net->FANN, result);
	}
	
	int dim = fann_get_num_output(net->FANN);
	Local<Array> result_arr = Array::New(dim);
	for (int i=0; i<dim; i++) {
		result_arr->Set(i, Number::New(result[i]));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> NNet::ActivationFunction(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	if (args.Length() < 2)
        return VException("Usage: func = activation_function(layer, neuron) or activation_function(layer, neuron, newfunc)");

	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);
	int layer = args[0]->IntegerValue();
	int neuron = args[1]->IntegerValue();

	if (args.Length() >= 3) {
		int num = -1;
		if (args[2]->IsString()) {
			num = _SeekCharArray(String::Cast(*args[2]), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
		} else if (args[2]->IsNumber()) {
			num = args[2]->NumberValue();
		}

		if (num >= 0 && num < size) {
			fann_set_activation_function(net->FANN, fann_activationfunc_enum(num), layer, neuron);
		}
	}

	enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, layer, neuron);
	if (func >= 0 && func < size) {
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, FANN_PREFIX_LEN));
	} else {
		return Undefined();
	}
}

Handle<Value> NNet::ActivationFunctionHidden(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

	if (args.Length() >= 1) {
		int num = -1;
		if (args[0]->IsString()) {
			num = _SeekCharArray(String::Cast(*args[0]), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
		} else if (args[0]->IsNumber()) {
			num = args[0]->NumberValue();
		}

		if (num >= 0 && num < size) {
			fann_set_activation_function_hidden(net->FANN, fann_activationfunc_enum(num));
		}
	}

	enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, 1, 0);
	if (func >= 0 && func < size) {
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, FANN_PREFIX_LEN));
	} else {
		return Undefined();
	}
}

Handle<Value> NNet::ActivationFunctionOutput(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

	if (args.Length() >= 1) {
		int num = -1;
		if (args[0]->IsString()) {
			num = _SeekCharArray(String::Cast(*args[0]), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
		} else if (args[0]->IsNumber()) {
			num = args[0]->NumberValue();
		}

		if (num >= 0 && num < size) {
			fann_set_activation_function_output(net->FANN, fann_activationfunc_enum(num));
		}
	}

	enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, fann_get_num_layers(net->FANN)-1, 0);
	if (func >= 0 && func < size) {
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, FANN_PREFIX_LEN));
	} else {
		return Undefined();
	}
}

void NNet::PrototypeInit(Local<FunctionTemplate> t)
{
	t->InstanceTemplate()->SetInternalFieldCount(1);
	NODE_SET_PROTOTYPE_METHOD(t, "train", Train);
	NODE_SET_PROTOTYPE_METHOD(t, "train_once", TrainOnce);
	NODE_SET_PROTOTYPE_METHOD(t, "run", Run);
	NODE_SET_PROTOTYPE_METHOD(t, "get_training_algorithms", GetTrainingAlgorithmList);
	NODE_SET_PROTOTYPE_METHOD(t, "get_activation_functions", GetActivationFunctionList);
	NODE_SET_PROTOTYPE_METHOD(t, "activation_function", ActivationFunction);
	NODE_SET_PROTOTYPE_METHOD(t, "activation_function_hidden", ActivationFunctionHidden);
	NODE_SET_PROTOTYPE_METHOD(t, "activation_function_output", ActivationFunctionOutput);
	t->InstanceTemplate()->SetAccessor(String::New("training_algorithm"), GetTrainingAlgorithm, SetTrainingAlgorithm);
	t->InstanceTemplate()->SetAccessor(String::New("learning_rate"), GetLearningRate, SetLearningRate);
	t->InstanceTemplate()->SetAccessor(String::New("learning_momentum"), GetLearningMomentum, SetLearningMomentum);
}

void NNet::Initialize(Handle<Object> target)
{
	HandleScope scope;
	Local<FunctionTemplate> t1 = FunctionTemplate::New(NewStandard);
	Local<FunctionTemplate> t2 = FunctionTemplate::New(NewSparse);
	Local<FunctionTemplate> t3 = FunctionTemplate::New(NewShortcut);
//	Local<FunctionTemplate> t4 = FunctionTemplate::New(CloneNet);
	PrototypeInit(t1);
	PrototypeInit(t2);
	PrototypeInit(t3);
//	PrototypeInit(t4);
	target->Set(String::NewSymbol("standard"), t1->GetFunction());
	target->Set(String::NewSymbol("shortcut"), t2->GetFunction());
	target->Set(String::NewSymbol("sparse"), t3->GetFunction());
//	target->Set(String::NewSymbol("clone"), t4->GetFunction());
	target->Set(String::NewSymbol("get_training_algorithms"), FunctionTemplate::New(GetTrainingAlgorithmList)->GetFunction());
	target->Set(String::NewSymbol("get_activation_functions"), FunctionTemplate::New(GetActivationFunctionList)->GetFunction());
}

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	NNet::Initialize(target);
}

