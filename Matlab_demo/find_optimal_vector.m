function x = find_optimal_vector(a, b)

    a_t = a';
    % A_T*A * x = A_T * b
    A = a_t*a;
    B = a_t*b;
    x = (A^-1) * B;

end