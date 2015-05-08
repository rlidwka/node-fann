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
  NanScope();
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

NAN_METHOD(NNet::NewStandard)
{
  NanScope();
  NNet *net = new NNet();
  net->Wrap(args.This());
  net->CreateStandard(args);
  NanReturnValue(args.This());
}

NAN_METHOD(NNet::NewSparse)
{
  NanScope();
  NNet *net = new NNet();
  net->Wrap(args.This());
  net->CreateSparse(args);
  NanReturnValue(args.This());
}

NAN_METHOD(NNet::NewShortcut)
{
  NanScope();
  NNet *net = new NNet();
  net->Wrap(args.This());
  net->CreateShortcut(args);
  NanReturnValue(args.This());
}

NAN_METHOD(NNet::NewFromFile)
{
  NanScope();
  NNet *net = new NNet();
  net->Wrap(args.This());
  net->CreateFromFile(args);
  NanReturnValue(args.This());
}

/* for FANN >= 2.2.0
NAN_METHOD(NNet::CloneNet)
{
    NanScope();
//    if (!NNet::HasInstance(args[0]))
//      return NanThrowError("First argument must be existing network.");
  NNet *net = new NNet();
  net->Wrap(args.This());
  net->CreateClone(args);
    NanReturnValue(args.This());
}*/

NAN_METHOD(NNet::CreateStandard)
{
  unsigned int* layers = NULL;
  int len = 0;

  if (args.Length() < 1)
    return NanThrowError("No arguments supplied");

  if (args[0]->IsArray()) {
    len = _GetLayersFromArray(layers, args[0].As<Array>());
  } else {
    Local<Array> arr = NanNew<Array>();
    for (int i=0; i<args.Length(); i++) {
      arr->Set(i, args[i]);
    }
    len = _GetLayersFromArray(layers, arr);
  }
  if (len <= 0) {
    if (layers != NULL) delete[] layers;
    return NanThrowError("Wrong arguments supplied");
  }

  FANN = fann_create_standard_array(len, layers);
  if (FANN == NULL)
    return NanThrowError("Failed to create neural network");

/*const float desired_error = (const float) 0.001;
const unsigned int max_epochs = 500000;
const unsigned int epochs_between_reports = 1000;
fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

fann_train_on_file(ann, "xor.data", max_epochs, epochs_between_reports, desired_error);

fann_save(ann, "xor_float.net");

fann_destroy(ann);*/
  delete[] layers;
  NanReturnUndefined();
}

NAN_METHOD(NNet::CreateSparse)
{
  unsigned int* layers = NULL;
  int len = 0;

  if (args.Length() < 1)
    return NanThrowError("No arguments supplied");

  if (!args[0]->IsNumber())
    return NanThrowError("First argument should be float");

  if (args[1]->IsArray()) {
    len = _GetLayersFromArray(layers, args[1].As<Array>());
  } else {
    Local<Array> arr = NanNew<Array>();
    /* skip 1st argument here */
    for (int i=1; i<args.Length(); i++) {
      arr->Set(i-1, args[i]);
    }
    len = _GetLayersFromArray(layers, arr);
  }
  if (len <= 0) {
    if (layers != NULL) delete[] layers;
    return NanThrowError("Wrong arguments supplied");
  }

  FANN = fann_create_sparse_array(args[0]->NumberValue(), len, layers);
  if (FANN == NULL)
    return NanThrowError("Failed to create neural network");

  delete[] layers;
  NanReturnUndefined();
}

NAN_METHOD(NNet::CreateShortcut)
{
  unsigned int* layers = NULL;
  int len = 0;

  if (args.Length() < 1)
    return NanThrowError("No arguments supplied");

  if (args[0]->IsArray()) {
    len = _GetLayersFromArray(layers, args[0].As<Array>());
  } else {
    Local<Array> arr = NanNew<Array>();
    for (int i=0; i<args.Length(); i++) {
      arr->Set(i, args[i]);
    }
    len = _GetLayersFromArray(layers, arr);
  }
  if (len <= 0) {
    if (layers != NULL) delete[] layers;
    return NanThrowError("Wrong arguments supplied");
  }

  FANN = fann_create_shortcut_array(len, layers);
  if (FANN == NULL)
    return NanThrowError("Failed to create neural network");

  delete[] layers;
  NanReturnUndefined();
}

NAN_METHOD(NNet::CreateFromFile)
{
  NanScope();
  if (args.Length() != 1 || !args[0]->IsString())
    return NanThrowError("usage: new FANN.load(\"filename.nnet\")");

  String::Utf8Value name(args[0].As<String>());

  FANN = fann_create_from_file(*name);
  if (FANN == NULL)
    return NanThrowError("Failed to create neural network");

  NanReturnUndefined();
}

NAN_METHOD(NNet::SaveToFile)
{
  NanScope();
  if (args.Length() != 1 || !args[0]->IsString())
    return NanThrowError("usage: net.save(\"filename.nnet\")");

  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  String::Utf8Value name(args[0].As<String>());

  fann_save(net->FANN, *name);
  NanReturnUndefined();
}

/* for FANN >= 2.2.0
NAN_METHOD(NNet::CreateClone)
{
  NNet *currnet = ObjectWrap::Unwrap<NNet>(args.This());
  NNet *oldnet = ObjectWrap::Unwrap<NNet>(args[0]->ToObject());
  currnet->FANN = fann_copy(oldnet->FANN);
//  printf("!!!!!!!!!!!!!! %d %d\n", currnet->something, oldnet->something);
  NanReturnUndefined();
}*/

NAN_METHOD(NNet::Run)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  if (args.Length() < 1)
    return NanThrowError("No arguments supplied");
  if (!args[0]->IsArray())
    return NanThrowError("First argument should be array");

  Local<Array> datain = args[0].As<Array>();
  fann_type *dataset_in = new fann_type[datain->Length()];
  for (unsigned i=0; i<datain->Length(); i++) {
    dataset_in[i] = datain->Get(i)->NumberValue();
  }

  fann_type *result = fann_run(net->FANN, dataset_in);

  if (net->scale_present) {
    fann_descale_output(net->FANN, result);
  }

  int dim = fann_get_num_output(net->FANN);
  Local<Array> result_arr = NanNew<Array>(dim);
  for (int i=0; i<dim; i++) {
    result_arr->Set(i, NanNew<Number>(result[i]));
  }

  NanReturnValue(result_arr);
}

