# Transpose a transposed vector. Should be optimized away by SimplifyRedundantTranspose
def transpose_transpose(a) {
  return transpose(transpose(a));
}

