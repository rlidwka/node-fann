/*
 *   All functions related to creation and running a network
 */

#include "node-fann.h"

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

int NNet::_GetLayersFromArray(unsigned int *&layers, Local<Array> a)
{
	HandleScope scope;
	int len = a->Length();
	if (len < 2)
		return 0;

	layers = new unsigned int[len];
	for (unsigned i=0; i<a->Length(); i++) {
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

Handle<Value> NNet::NewFromFile(const Arguments &args)
{
	HandleScope scope;
	NNet *net = new NNet();
	net->Wrap(args.This());
	net->CreateFromFile(args);
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

Handle<Value> NNet::CreateFromFile(const Arguments &args)
{
	HandleScope scope;
	if (args.Length() != 1 || !args[0]->IsString())
		return VException("usage: new FANN.load(\"filename.nnet\")");
	
	char name[4096];
	String::Cast(*args[0])->WriteAscii(name, 0, 4096);
	name[4095] = 0;

	FANN = fann_create_from_file(name);
	return Undefined();
}

Handle<Value> NNet::SaveToFile(const Arguments &args)
{
	HandleScope scope;
	if (args.Length() != 1 || !args[0]->IsString())
		return VException("usage: net.save(\"filename.nnet\")");
	
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	char name[4096];
	String::Cast(*args[0])->WriteAscii(name, 0, 4096);
	name[4095] = 0;

	fann_save(net->FANN, name);
	return Undefined();
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
	for (unsigned i=0; i<datain->Length(); i++) {
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
