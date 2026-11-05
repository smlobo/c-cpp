module {
  toy.func @main() {
    %0 = toy.constant dense<[[1.000000e+00], [2.000000e+00]]> : tensor<2x1xf64>
    %1 = toy.cast %0 : tensor<2x1xf64> to tensor<*xf64>
    toy.print %1 : tensor<*xf64>
    toy.return
  }
}

