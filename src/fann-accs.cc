/*
 *   All setters, getters and other information providers
 */

#include <string.h>
#include <stdio.h>
#include "node-fann.h"

NAN_GETTER(NNet::GetTrainingAlgorithm)
{
  NanScope();
  Local<Object> self = args.Holder();
  NNet *net = ObjectWrap::Unwrap<NNet>(self);
  int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);
  enum fann_train_enum algo = fann_get_training_algorithm(net->FANN);

  if (algo >= 0 && algo < size) {
    NanReturnValue(NormalizeName(FANN_TRAIN_NAMES[algo], TRAIN_PREFIX, sizeof(TRAIN_PREFIX)-1));
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(NNet::GetNetworkType)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  int size = sizeof(FANN_NETTYPE_NAMES)/sizeof(char*);
  enum fann_nettype_enum ret = fann_get_network_type(net->FANN);

  if (ret >= 0 && ret < size) {
    NanReturnValue(NormalizeName(FANN_NETTYPE_NAMES[ret], NETTYPE_PREFIX, sizeof(NETTYPE_PREFIX)-1));
  } else {
    NanReturnUndefined();
  }
}

NAN_SETTER(NNet::SetTrainingAlgorithm)
{
  NanScope();
  Local<Object> self = args.Holder();
  NNet *net = ObjectWrap::Unwrap<NNet>(self);
  int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);
  int num = -1;

  if (value->IsString()) {
    num = _SeekCharArray(value.As<String>(), FANN_TRAIN_NAMES, size, TRAIN_PREFIX);
  } else if (value->IsNumber()) {
    num = value->NumberValue();
  }

  if (num >= 0 && num < size) {
    fann_set_training_algorithm(net->FANN, fann_train_enum(num));
  }
}

NAN_GETTER(NNet::GetLearningRate)
{
  NanScope();
  Local<Object> self = args.Holder();
  NNet *net = ObjectWrap::Unwrap<NNet>(self);

  float rate = fann_get_learning_rate(net->FANN);
  NanReturnValue(NanNew<Number>(rate));
}

NAN_GETTER(NNet::GetLearningMomentum)
{
  NanScope();
  Local<Object> self = args.Holder();
  NNet *net = ObjectWrap::Unwrap<NNet>(self);

  float momentum = fann_get_learning_momentum(net->FANN);
  NanReturnValue(NanNew<Number>(momentum));
}

NAN_SETTER(NNet::SetLearningRate)
{
  NanScope();
  Local<Object> self = args.Holder();
  NNet *net = ObjectWrap::Unwrap<NNet>(self);

  fann_set_learning_rate(net->FANN, value->NumberValue());
}

NAN_SETTER(NNet::SetLearningMomentum)
{
  NanScope();
  Local<Object> self = args.Holder();
  NNet *net = ObjectWrap::Unwrap<NNet>(self);

  fann_set_learning_momentum(net->FANN, value->NumberValue());
}

NAN_METHOD(NNet::ActivationFunction)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  if (args.Length() < 2)
    return NanThrowError("Usage: func = activation_function(layer, neuron) or activation_function(layer, neuron, newfunc)");

  int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);
  int layer = args[0]->IntegerValue();
  int neuron = args[1]->IntegerValue();

  if (args.Length() >= 3) {
    int num = -1;
    if (args[2]->IsString()) {
      num = _SeekCharArray(args[2].As<String>(), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
    } else if (args[2]->IsNumber()) {
      num = args[2]->NumberValue();
    }

    if (num >= 0 && num < size) {
      fann_set_activation_function(net->FANN, fann_activationfunc_enum(num), layer, neuron);
    }
  }

  enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, layer, neuron);
  if (func >= 0 && func < size) {
    NanReturnValue(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, sizeof(FANN_PREFIX)-1));
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(NNet::ActivationFunctionHidden)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

  if (args.Length() >= 1) {
    int num = -1;
    if (args[0]->IsString()) {
      num = _SeekCharArray(args[0].As<String>(), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
    } else if (args[0]->IsNumber()) {
      num = args[0]->NumberValue();
    }

    if (num >= 0 && num < size) {
      fann_set_activation_function_hidden(net->FANN, fann_activationfunc_enum(num));
    }
  }

  enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, 1, 0);
  if (func >= 0 && func < size) {
    NanReturnValue(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, sizeof(FANN_PREFIX)-1));
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(NNet::ActivationFunctionOutput)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

  if (args.Length() >= 1) {
    int num = -1;
    if (args[0]->IsString()) {
      num = _SeekCharArray(args[0].As<String>(), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
    } else if (args[0]->IsNumber()) {
      num = args[0]->NumberValue();
    }

    if (num >= 0 && num < size) {
      fann_set_activation_function_output(net->FANN, fann_activationfunc_enum(num));
    }
  }

  enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, fann_get_num_layers(net->FANN)-1, 0);
  if (func >= 0 && func < size) {
    NanReturnValue(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, sizeof(FANN_PREFIX)-1));
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(NNet::GetMse)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  float ret = fann_get_MSE(net->FANN);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(NNet::GetNumInput)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  unsigned int ret = fann_get_num_input(net->FANN);
  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(NNet::GetNumOutput)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  unsigned int ret = fann_get_num_output(net->FANN);
  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(NNet::GetTotalNeurons)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  unsigned int ret = fann_get_total_neurons(net->FANN);
  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(NNet::GetTotalConnections)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  unsigned int ret = fann_get_total_connections(net->FANN);
  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(NNet::GetConnectionRate)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  float ret = fann_get_connection_rate(net->FANN);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(NNet::GetNumLayers)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  unsigned int ret = fann_get_num_layers(net->FANN);
  NanReturnValue(NanNew<Integer>(ret));
}

NAN_GETTER(NNet::GetLayerArray)
{
  NanScope();
  Local<Object> self = args.Holder();
  NNet *net = ObjectWrap::Unwrap<NNet>(self);
  NanReturnValue(net->GetLayers());
}

NAN_METHOD(NNet::GetLayerArray)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  NanReturnValue(net->GetLayers());
}

Local<Array> NNet::GetLayers()
{
  int size = fann_get_num_layers(FANN);
  unsigned int* layers = new unsigned int[size];
  fann_get_layer_array(FANN, layers);

  Local<Array> result_arr = NanNew<Array>();
  for (int i=0; i<size; i++) {
    result_arr->Set(i, NanNew<Number>(layers[i]));
  }

  delete[] layers;
  return result_arr;
}

NAN_METHOD(NNet::GetBiasArray)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  int size = fann_get_num_layers(net->FANN);
  unsigned int* layers = new unsigned int[size];
  fann_get_bias_array(net->FANN, layers);

  Local<Array> result_arr = NanNew<Array>();
  for (int i=0; i<size; i++) {
    result_arr->Set(i, NanNew<Number>(layers[i]));
  }

  delete[] layers;
  NanReturnValue(result_arr);
}

NAN_METHOD(NNet::GetWeights)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  int size = fann_get_total_connections(net->FANN);
  struct fann_connection *conns = new struct fann_connection[size];
  fann_get_connection_array(net->FANN, conns);

  Local<Object> result_object = NanNew<Object>();
  for (int i=0; i<size; i++) {
    Local<Object> obj;
    if (!result_object->Has(conns[i].from_neuron)) {
      obj = NanNew<Object>();
      result_object->Set(conns[i].from_neuron, obj);
    } else {
      obj = result_object->Get(conns[i].from_neuron).As<Object>();
    }
    obj->Set(conns[i].to_neuron, NanNew<Number>(conns[i].weight));
  }

  delete[] conns;
  NanReturnValue(result_object);
}

NAN_METHOD(NNet::SetWeightsArr)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

  if (!args[0]->IsObject())
    return NanThrowError("First argument should be object");
  Local<Array> arg = args[0].As<Array>();
  Local<Array> keys = arg->GetOwnPropertyNames();

  struct fann_connection *conns = new struct fann_connection[fann_get_total_connections(net->FANN)];
  int counter = 0;
  for (unsigned i=0; i<keys->Length(); i++) {
    Local<Value> idx = keys->Get(i);
    if (!arg->Get(idx)->IsObject()) continue;
    Local<Object> obj = arg->Get(idx).As<Object>();
    Local<Array> keys2 = obj->GetOwnPropertyNames();
    for (unsigned j=0; j<keys2->Length(); j++) {
      conns[counter].from_neuron = idx->IntegerValue();
      conns[counter].to_neuron = keys2->Get(j)->IntegerValue();
      conns[counter].weight = obj->Get(keys2->Get(j))->NumberValue();
      counter++;
    }
  }
  fann_set_weight_array(net->FANN, conns, counter);

  delete[] conns;
  NanReturnUndefined();
}

NAN_METHOD(NNet::SetWeights)
{
  NanScope();
  if (args[0]->IsObject())
    return SetWeightsArr(args);

  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  if (args.Length() < 3)
    return NanThrowError("Usage: set_weights(new_object) or set_weight(from_neuron, to_neuron, weight)");

  unsigned int from_neuron = args[0]->IntegerValue();
  unsigned int to_neuron = args[1]->IntegerValue();
  fann_type weight = args[2]->NumberValue();

  fann_set_weight(net->FANN, from_neuron, to_neuron, weight);
  NanReturnUndefined();
}

