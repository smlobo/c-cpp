//
// Created by Sheldon Lobo on 11/4/26.
//

#include "Dialect.h"

#include "mlir/IR/PatternMatch.h"

/// Simplify reshape x : tensor<y> to tensor<y> -> x
struct SimplifyRedundantReshape : public mlir::OpRewritePattern<mlir::toy::ReshapeOp> {
    /// We register this pattern to match every toy.reshape in the IR.
    /// The "benefit" is used by the framework to order the patterns and process
    /// them in order of profitability.
    SimplifyRedundantReshape(mlir::MLIRContext *context)
        : OpRewritePattern<mlir::toy::ReshapeOp>(context, /*benefit=*/1) {}

    /// This method is attempting to match a pattern and rewrite it. The rewriter
    /// argument is the orchestrator of the sequence of rewrites. It is expected
    /// to interact with it to perform any changes to the IR from here.
    llvm::LogicalResult
    matchAndRewrite(mlir::toy::ReshapeOp op, mlir::PatternRewriter &rewriter) const override {
        // Input of the reshape.
        mlir::Value reshapeInput = op.getOperand();
        auto reshapeInputType = llvm::dyn_cast<mlir::RankedTensorType>(reshapeInput.getType());
        if (!reshapeInputType || !reshapeInputType.hasStaticShape())
            return mlir::failure();

        // Result of the reshape.
        mlir::Value reshapeResult = op.getResult();
        auto reshapeResultType = llvm::dyn_cast<mlir::RankedTensorType>(reshapeResult.getType());
        if (!reshapeResultType || !reshapeResultType.hasStaticShape())
            return mlir::failure();

        if (!reshapeInputType.getShape().equals(reshapeResultType.getShape()))
            return mlir::failure();

        // Redundant reshape.
        rewriter.replaceOp(op, {reshapeInput});

        return mlir::success();
    }
};

/// Register SimplifyRedundantReshape as "canonicalization" patterns on the ReshapeOp
void addSimplifyRedundantReshapePattern(mlir::RewritePatternSet &results, mlir::MLIRContext *context) {
    results.add<SimplifyRedundantReshape>(context);
}
