module {
  toy.func @return_add(%arg0: tensor<*xf64>) -> tensor<*xf64> {
    %0 = toy.add %arg0, %arg0 : tensor<*xf64>
    toy.return %0 : tensor<*xf64>
  }
}

