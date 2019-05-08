#include "caffe/caffe.hpp"
#include "caffe/util/io.hpp"
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace caffe;
using ::google::protobuf::Message;
using ::boost::filesystem::path;
using namespace std;
float weight[] = {0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111};
void caffe_forward(boost::shared_ptr< Net<float> > & net, float *data_ptr)
{
    Blob<float>* input_blobs = net->input_blobs()[0];
    switch (Caffe::mode())
    {
    case Caffe::CPU:
        memcpy(input_blobs->mutable_cpu_data(), data_ptr,
            sizeof(float) * input_blobs->count());
        break;
   /* case Caffe::GPU:
        cudaMemcpy(input_blobs->mutable_gpu_data(), data_ptr,
            sizeof(float) * input_blobs->count(), cudaMemcpyHostToDevice);
        break;*/
    default:
        LOG(FATAL) << "Unknown Caffe mode.";
    } 
    net->ForwardPrefilled();
}

unsigned int get_layer_index(boost::shared_ptr< Net<float> > & net, const char *query_layer_name)
{
    std::string str_query(query_layer_name);    
    vector< string > const & layer_names = net->layer_names();
    for( unsigned int i = 0; i != layer_names.size(); ++i ) 
    { 
        if( str_query == layer_names[i] ) 
        { 
            return i;
        } 
    }
    LOG(FATAL) << "Unknown layer name: " << str_query;
}

void writeBinaryFile(NetParameter &net_param,const  char* weights_file)
{
    WriteProtoToBinaryFile(net_param, weights_file);
}

unsigned int get_blob_index(boost::shared_ptr< Net<float> > & net,const  char *query_blob_name)
{
    std::string str_query(query_blob_name);    
    vector< string > const & blob_names = net->blob_names();
    for( unsigned int i = 0; i != blob_names.size(); ++i ) 
    { 
        if( str_query == blob_names[i] ) 
        { 
            return i;
        } 
    }
    LOG(FATAL) << "Unknown blob name: " << str_query;
}
void* readWeightByLayerName(boost::shared_ptr< Net<float> > & net,const char *query_blob_name)
{
    unsigned int layer_id = get_layer_index(net, query_blob_name);
    boost::shared_ptr<Layer<float> > layer = net->layers()[layer_id];
    std::vector<boost::shared_ptr<Blob<float>  >> blobs = layer->blobs();
    void *w ;
    if (blobs.size() > 0)
    {    
       w =  blobs[0]->mutable_cpu_data();
    }
    return w;    
}
void* readBiasByLayerName(boost::shared_ptr< Net<float> > & net,const char *query_blob_name)
{
    unsigned int layer_id = get_layer_index(net, query_blob_name);
    boost::shared_ptr<Layer<float> > layer = net->layers()[layer_id];
    std::vector<boost::shared_ptr<Blob<float>  >> blobs = layer->blobs();
    void *b ;
    if (blobs.size() > 1)
    {    
       b =  blobs[1]->mutable_cpu_data();
    }
    return b;    
}
void writeModelWeightData(boost::shared_ptr< Net<float> > & net,NetParameter &param,const char *layer_name,const float* data_ptr)
{
              /* 指向待写入数据的指针， 源数据指针*/
    float* weight_ptr = NULL;
    unsigned int layer_id = get_layer_index(net, layer_name);    
    caffe::LayerParameter* layerparam=param.mutable_layer(layer_id);
    ConvolutionParameter* conv_param = param.mutable_layer(layer_id)->mutable_convolution_param();
    if (conv_param->has_weight_filler())
    {
        FillerParameter* weight_f = conv_param->mutable_weight_filler();
        if (weight_f->has_type())
        {
	    cout << weight_f->type() << endl;
        }
    }

    BlobProto *weight_blob = layerparam->mutable_blobs(0);
    printf("weight NCHW=(%d %d %d %d)\n",weight_blob->num(),weight_blob->channels(),weight_blob->height(),weight_blob->width());
    BlobShape blobshape = weight_blob->shape();
    if (weight_blob->has_shape())
    {
	printf("weight shape = (%d %d %d %d)",blobshape.dim(0),blobshape.dim(1),blobshape.dim(2),blobshape.dim(3));
    }
    for (int i = 0; i < (blobshape.dim(0)*blobshape.dim(1)*blobshape.dim(2)*blobshape.dim(3)); i++)
    {
	weight_blob->set_data(i, data_ptr[i]);
	cout << "dst:" << weight_blob->data(i) << endl;
	if(i == 10)
        {
	    break;
        }
    }
}

void writeModelBiasData(boost::shared_ptr< Net<float> > & net,const char *layer_name,const float* data_ptr)
{
              /* 指向待写入数据的指针， 源数据指针*/
    unsigned int layer_id = get_layer_index(net, layer_name); 
    float* bias_ptr = NULL;   
    boost::shared_ptr<Blob<float> > blob = net->layers()[layer_id]->blobs()[1];
    switch (Caffe::mode())
    {
        case Caffe::CPU:
       // dataBuf = 
        bias_ptr = blob->mutable_cpu_data();
        break;
        case Caffe::GPU:
        bias_ptr = blob->mutable_gpu_data();
        break;
        default:
        LOG(FATAL) << "Unknown Caffe mode";
    }
    caffe_copy(blob->count(), data_ptr, bias_ptr);
}

int main(int argc,char** argv)
{
    char *proto = argv[1]; /* 加载CaffeNet的配置 */
    Phase phase = TEST; /* or TRAIN */
    Caffe::set_mode(Caffe::CPU);

    boost::shared_ptr< Net<float> > net(new caffe::Net<float>(proto, phase));
 
    const char *model = argv[2];
    net->CopyTrainedLayersFrom(model);
    NetParameter param;
    ReadNetParamsFromBinaryFileOrDie(model, &param);
    caffe::LayerParameter* layerparam=NULL;
    int num_layers = param.layer_size();
    printf("num_layer= %d\n",num_layers);
    float* data_ptr = weight;
    writeModelWeightData(net,param,"conv1/3x3_s1",data_ptr); 
 //   writeBinaryFile(net,"trt2.caffemodel");
    for (int i = 0; i < num_layers; ++i)
    {
        layerparam = param.mutable_layer(i);
       // std::cout<< "Layer_" << i << ":" << param.layer(i).name()<<";"<<"type:"<< param.layer(i).type() << "\n";
        if (layerparam->type() == "Convolution")
        {
            void* data;
            data=readWeightByLayerName(net,param.layer(i).name().c_str());
            float d = *(float *)data;
            printf("\n %f \n",d);
        }
    }
    string dst_name =string(argv[2]) + string("_dst")+string(".caffemodel");
    writeBinaryFile(param,dst_name.c_str());
//    WriteProtoToTextFile(param,"param.txt");

}

