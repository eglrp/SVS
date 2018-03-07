#ifndef CAFFE_SLICING_LAYER_HPP_
#define CAFFE_SLICING_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include <algorithm>
#include <math.h>
#include <stdlib.h>

namespace caffe {


// """Slices into a bilateral grid using the guide map.

//   Args:
  // guide: (Tensor) [batch_size, h, w ] guide map to slice along.
//     grid: (Tensor) [batch_size, grid_h, grid_w, depth * n_outputs]
//       grid to slice from.
//     
//     name: (string) name for the operation.
//   Returns:
//     sliced: (Tensor) [batch_size, h, w, n_outputs] sliced output.
//   """

/**
 * @brief  Migrate the tensorflow implementation to caffe
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */
template <typename Dtype>
class BilateralSlicingLayer : public Layer<Dtype> {
 public:
  explicit BilateralSlicingLayer(const LayerParameter& param)
      : Layer<Dtype>(param) {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline const char* type() const { return "BilateralSlicing"; }
  virtual inline int ExactNumBottomBlobs() const { return 2; }  // bottom[0] is guidance map g, value in range of [0,1], in shape of [N, 1, H,W]
                                                                // bottom[1] is bilateral grid, in shape of [N, D * stride, H', w']
                                                                // NOTE that x is along Width, y is along height.
  virtual inline int ExactNumTopBlobs() const { return 1; }     // top[0] is the tri-linearly interpolating coefficients

 protected:

  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  // Note the function does not have differential value when scale = 0
  inline Dtype tao_f(Dtype scale) const {return std::max(0.0, 1 - fabs(scale)); };
  Dtype scale_x_;
  Dtype scale_y_;
  Dtype depth_d_;
  int coefficient_len_;
  int offset_x_;
  int offset_y_;
  int offset_z_;
  


};

}  // namespace caffe

#endif  // CAFFE_SLICING_LAYER_HPP_
