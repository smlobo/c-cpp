//
// Created by Sheldon Lobo on 11/5/26.
//

#include "Dialect.h"

void addSimplifyRedundantReshapePattern(mlir::RewritePatternSet &results, mlir::MLIRContext *context);
void addFoldConstantReshapePattern(mlir::RewritePatternSet &results, mlir::MLIRContext *context);

/// Register our patterns as "canonicalization" patterns on the ReshapeOp so
/// that they can be picked up by the Canonicalization framework.
void mlir::toy::ReshapeOp::getCanonicalizationPatterns(RewritePatternSet &results, MLIRContext *context) {
    addSimplifyRedundantReshapePattern(results, context);
    addFoldConstantReshapePattern(results, context);
}
