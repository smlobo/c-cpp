module {
  toy.func @variable_declaration() -> tensor<6xf64> {
    %0 = toy.constant dense<[1.000000e+00, 2.000000e+00, 3.000000e+00, 4.000000e+00, 5.000000e+00, 6.000000e+00]> : tensor<6xf64>
    toy.return %0 : tensor<6xf64>
  }
}

