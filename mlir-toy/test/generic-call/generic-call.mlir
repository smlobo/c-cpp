module {
  toy.func private @get_vector() -> tensor<2x1xf64> {
    %0 = toy.constant dense<[1.000000e+00, 2.000000e+00]> : tensor<2xf64>
    %1 = toy.reshape(%0 : tensor<2xf64>) to tensor<2x1xf64>
    toy.return %1 : tensor<2x1xf64>
  }
  toy.func @main() {
    %0 = toy.generic_call @get_vector() : () -> tensor<*xf64>
    toy.print %0 : tensor<*xf64>
    toy.return
  }
}

