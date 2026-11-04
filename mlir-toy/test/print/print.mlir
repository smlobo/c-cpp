module {
  toy.func @call_print() -> tensor<2x3xf64> {
    %0 = toy.constant dense<[1.000000e+00, 2.000000e+00, 3.000000e+00, 4.000000e+00, 5.000000e+00, 6.000000e+00]> : tensor<6xf64>
    %1 = toy.reshape(%0 : tensor<6xf64>) to tensor<2x3xf64>
    toy.print %1 : tensor<2x3xf64>
    toy.return %1 : tensor<2x3xf64>
  }
}

