//this file was generated by ../../../../../../scripts/onnx_generator/OperatorHeader.py
# ifndef OPERATOR_OPERATOR__AI_ONNX__RELU__6_H
# define OPERATOR_OPERATOR__AI_ONNX__RELU__6_H

# include "operators/operator.h"
# include "operators/operator_stub.h"
# include "operators/operator_info.h"

/**
 * ai.onnx operator 'Relu' version 6
 *
 * @param[in]  ctx  Operator context
 * @return          Status code
 *
 * Relu takes one input data (Tensor<T>) and produces one output data
 * (Tensor<T>) where the rectified linear function, y = max(0, x), is applied to
 * the tensor elementwise.
 * 
 * Constraint T:
 *   Constrain input and output types to float tensors.
 *   Allowed Types: tensor_double, tensor_float, tensor_float16
 * Input T X:
 *   Input tensor
 *   Allowed Types: tensor_double, tensor_float, tensor_float16
 * Output T Y:
 *   Output tensor
 *   Allowed Types: tensor_double, tensor_float, tensor_float16

 *
 * @since version 6
 *
 * @see io/onnx/onnx/defs/math/defs.cc:320
 * @see https://github.com/onnx/onnx/blob/master/docs/Operators.md#Relu
 */
operator_status operator__ai_onnx__relu__6(
    node_context *ctx
);
operator_status operator__ai_onnx__relu__6__T_tensor_double(
    node_context *ctx
);
operator_status operator__ai_onnx__relu__6__T_tensor_float(
    node_context *ctx
);
operator_status operator__ai_onnx__relu__6__T_tensor_float16(
    node_context *ctx
);

operator_executer resolve_operator__ai_onnx__relu__6(
    node_context *ctx
);

extern operator_info info_operator__ai_onnx__relu__6;
# endif