/*
 *   All functions related to cascade training
 */

#include "node-fann.h"

void NNet::CascadeTrainOnData(struct fann_train_data *traindata, unsigned int max_neurons, unsigned int neurons_between_reports, float desired_error)
{
  fann_cascadetrain_on_data(FANN, traindata, max_neurons, neurons_between_reports, desired_error);
}

NAN_METHOD(NNet::CascadeTrain)
{
  NanScope();
  NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
  struct fann_train_data *traindata = NULL;

  if (args.Length() < 1)
    return NanThrowError("No arguments supplied");

  if (!args[0]->IsArray())
    return NanThrowError("First argument should be 2d-array (training data set)");

  Local<Array> dataset = args[0].As<Array>();
  net->MakeTrainData(dataset, &traindata);

  if (traindata == NULL)
    return NanThrowError("Internal error");


  unsigned int max_neurons = 100000;
  unsigned int neurons_between_reports = 1000;
  float desired_error = 0.001;
  int scale = 0;
  if (args.Length() >= 2) {
    Local<Object> params = args[1].As<Object>();
    if (params->Has(NanNew<String>("neurons"))) {
      max_neurons = params->Get(NanNew<String>("neurons"))->IntegerValue();
    }
    if (params->Has(NanNew<String>("neurons_between_reports"))) {
      neurons_between_reports = params->Get(NanNew<String>("neurons_between_reports"))->IntegerValue();
    }
    if (params->Has(NanNew<String>("error"))) {
      desired_error = params->Get(NanNew<String>("error"))->NumberValue();
    }
    if (params->Has(NanNew<String>("scale"))) {
      scale = params->Get(NanNew<String>("scale"))->BooleanValue();
    }
  }
  if (scale) {
    fann_scale_train(net->FANN, traindata);
    net->scale_present = true;
  }
  net->CascadeTrainOnData(traindata, max_neurons, neurons_between_reports, desired_error);
  fann_destroy_train(traindata);
  NanReturnUndefined();
}

