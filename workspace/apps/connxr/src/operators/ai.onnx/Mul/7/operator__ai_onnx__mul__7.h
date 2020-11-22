//this file was generated by ../../../../../../scripts/onnx_generator/OperatorHeader.py
# ifndef OPERATOR_OPERATOR__AI_ONNX__MUL__7_H
# define OPERATOR_OPERATOR__AI_ONNX__MUL__7_H

# include "operators/operator.h"
# include "operators/operator_stub.h"
# include "operators/operator_info.h"

/**
 * ai.onnx operator 'Mul' version 7
 *
 * @param[in]  ctx  Operator context
 * @return          Status code
 *
 * Performs element-wise binary multiplication (with Numpy-style broadcasting support).
 * 
 * This operator supports **multidirectional (i.e., Numpy-style) broadcasting**; for more details please check [the doc](Broadcasting.md).
 * 
 * Constraint T:
 *   Constrain input and output types to high-precision numeric tensors.
 *   Allowed Types: tensor_double, tensor_float, tensor_float16, tensor_int32,
 *                  tensor_int64, tensor_uint32, tensor_uint64
 * Input T A:
 *   First operand.
 *   Allowed Types: tensor_double, tensor_float, tensor_float16, tensor_int32,
 *                  tensor_int64, tensor_uint32, tensor_uint64
 * 
 * Input T B:
 *   Second operand.
 *   Allowed Types: tensor_double, tensor_float, tensor_float16, tensor_int32,
 *                  tensor_int64, tensor_uint32, tensor_uint64
 * Output T C:
 *   Result, has same element type as two inputs
 *   Allowed Types: tensor_double, tensor_float, tensor_float16, tensor_int32,
 *                  tensor_int64, tensor_uint32, tensor_uint64

 *
 * @since version 7
 *
 * @see io/onnx/onnx/defs/math/defs.cc:176
 * @see https://github.com/onnx/onnx/blob/master/docs/Operators.md#Mul
 */
operator_status operator__ai_onnx__mul__7(
    node_context *ctx
);
operator_status operator__ai_onnx__mul__7__T_tensor_double(
    node_context *ctx
);
operator_status operator__ai_onnx__mul__7__T_tensor_float(
    node_context *ctx
);
operator_status operator__ai_onnx__mul__7__T_tensor_float16(
    node_context *ctx
);
operator_status operator__ai_onnx__mul__7__T_tensor_int32(
    node_context *ctx
);
operator_status operator__ai_onnx__mul__7__T_tensor_int64(
    node_context *ctx
);
operator_status operator__ai_onnx__mul__7__T_tensor_uint32(
    node_context *ctx
);
operator_status operator__ai_onnx__mul__7__T_tensor_uint64(
    node_context *ctx
);

operator_executer resolve_operator__ai_onnx__mul__7(
    node_context *ctx
);

extern operator_info info_operator__ai_onnx__mul__7;
# endif