/*
 *  Main FANN class
 */

#include "node-futil.h"

class NNet : public ObjectWrap
{
  public:
    int something;
    NNet();
    ~NNet();
    NAN_METHOD(CreateStandard);
    NAN_METHOD(CreateSparse);
    NAN_METHOD(CreateShortcut);
    NAN_METHOD(CreateFromFile);
    //NAN_METHOD(CreateClone);
    static void Initialize(Handle<Object> target);
    static NAN_METHOD(NewStandard);
    static NAN_METHOD(NewSparse);
    static NAN_METHOD(NewShortcut);
    static NAN_METHOD(NewFromFile);
    //static NAN_METHOD(CloneNet);
    static NAN_GETTER(GetTrainingAlgorithm);
    static NAN_SETTER(SetTrainingAlgorithm);
    static NAN_GETTER(GetLearningRate);
    static NAN_SETTER(SetLearningRate);
    static NAN_GETTER(GetLearningMomentum);
    static NAN_SETTER(SetLearningMomentum);
    static NAN_METHOD(ActivationFunction);
    static NAN_METHOD(ActivationFunctionHidden);
    static NAN_METHOD(ActivationFunctionOutput);
    static NAN_METHOD(GetNumInput);
    static NAN_METHOD(GetNumOutput);
    static NAN_METHOD(GetMse);
    static NAN_METHOD(GetTotalNeurons);
    static NAN_METHOD(GetTotalConnections);
    static NAN_METHOD(GetNetworkType);
    static NAN_METHOD(GetConnectionRate);
    static NAN_METHOD(GetNumLayers);
    static NAN_GETTER(GetLayerArray);
    static NAN_METHOD(GetLayerArray);
    static NAN_METHOD(GetWeights);
    static NAN_METHOD(SetWeights);
    static NAN_METHOD(SetWeightsArr);
    static NAN_METHOD(GetBiasArray);
    static NAN_METHOD(Train);
    static NAN_METHOD(TrainOnce);
    static NAN_METHOD(Run);
    static NAN_METHOD(CascadeTrain);
    static NAN_METHOD(SaveToFile);

  private:
    struct fann *FANN;
    bool scale_present;
    int _GetLayersFromArray(unsigned int *&layers, Local<Array> a);
    static void PrototypeInit(Local<FunctionTemplate> t);
    const char* MakeTrainData(Local<Array> arr, struct fann_train_data **traindata);
    void TrainOnData(struct fann_train_data *traindata, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error);
    void CascadeTrainOnData(struct fann_train_data *traindata, unsigned int max_neurons, unsigned int neurons_between_reports, float desired_error);
    Local<Array> GetLayers();
};
