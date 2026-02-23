# Functional (recursive) style fibonacci

def fibf(x)
    if (x < 3) then
        1
    else
        fibf(x-1)+fibf(x-2)
