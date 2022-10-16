j = 1i;
g_n = [1 2 0 1];
h_n = [2 2 1 1];
x_n = g_n + j.*h_n;
N = 4;
W_N = exp(-j * 2*pi/N);
v = zeros(N,1);
F = zeros(N,N);
for d = 1:N
    v(d) = W_N^(d-1);
end

for t = 1:N
    F(:,t) = v.^(t-1);
end

%(1/sqrt(N).*F)^4
X_k = F*x_n'

figure
stem(1:N, X_k)
grid on