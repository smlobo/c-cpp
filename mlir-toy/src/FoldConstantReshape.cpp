//
// Created by Sheldon Lobo on 11/4/26.
//

#include "Dialect.h"

#include "mlir/IR/PatternMatch.h"

/// Fold reshape(constant) into a constant with the reshaped type.
struct FoldConstantReshape : public mlir::OpRewritePattern<mlir::toy::ReshapeOp> {
    /// We register this pattern to match every toy.reshape in the IR.
    /// The "benefit" is used by the framework to order the patterns and process
    /// them in order of profitability.
    FoldConstantReshape(mlir::MLIRContext *context)
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

        auto constantOp = reshapeInput.getDefiningOp<mlir::toy::ConstantOp>();
        if (!constantOp)
            return mlir::failure();

        // ConstantOp and ReshapeOp result have the same element count
        auto constantValue = constantOp.getValue();
        if (constantValue.getNumElements() != reshapeResultType.getNumElements())
            return mlir::failure();

        // Create a new ConstantOp, and replace the ReshapeOp with its result
        // The original ConstantOp will get dead-code cleaned up if there are no other uses.
        auto reshapedValue = constantValue.reshape(reshapeResultType);
        auto foldedConstant = mlir::toy::ConstantOp::create(
            rewriter, op.getLoc(), reshapedValue);
        rewriter.replaceOp(op, foldedConstant.getResult());

        return mlir::success();
    }
};

/// Register FoldConstantReshape as "canonicalization" patterns on the ReshapeOp
void addFoldConstantReshapePattern(mlir::RewritePatternSet &results, mlir::MLIRContext *context) {
    results.add<FoldConstantReshape>(context);
}
