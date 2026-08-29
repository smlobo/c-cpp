//
// Created by Sheldon Lobo on 8/25/26.
//

#include "Dialect.h"

#include "mlir/IR/PatternMatch.h"

/// Fold transpose(transpose(x)) -> x
struct SimplifyRedundantTranspose : public mlir::OpRewritePattern<mlir::toy::TransposeOp> {
    /// We register this pattern to match every toy.transpose in the IR.
    /// The "benefit" is used by the framework to order the patterns and process
    /// them in order of profitability.
    SimplifyRedundantTranspose(mlir::MLIRContext *context)
        : OpRewritePattern<mlir::toy::TransposeOp>(context, /*benefit=*/1) {}

    /// This method is attempting to match a pattern and rewrite it. The rewriter
    /// argument is the orchestrator of the sequence of rewrites. It is expected
    /// to interact with it to perform any changes to the IR from here.
    llvm::LogicalResult
    matchAndRewrite(mlir::toy::TransposeOp op,
                    mlir::PatternRewriter &rewriter) const override {
        // Look through the input of the current transpose.
        mlir::Value transposeInput = op.getOperand();
        mlir::toy::TransposeOp transposeInputOp =
            transposeInput.getDefiningOp<mlir::toy::TransposeOp>();

        // Input defined by another transpose? If not, no match.
        if (!transposeInputOp)
            return mlir::failure();

        // Otherwise, we have a redundant transpose. Use the rewriter.
        rewriter.replaceOp(op, {transposeInputOp.getOperand()});
        return mlir::success();
    }
};

/// Register our patterns as "canonicalization" patterns on the TransposeOp so
/// that they can be picked up by the Canonicalization framework.
void mlir::toy::TransposeOp::getCanonicalizationPatterns(RewritePatternSet &results,
                                                         MLIRContext *context) {
    results.add<SimplifyRedundantTranspose>(context);
}
