clc;
clear;
close all;

d_ang = 0.01;
fs = 10;                                %取樣率(DTFT使用)
ts = 1/fs;                              %取樣週期(DTFT使用)


h = load('h.txt');
[X1,x1,df1] = fftseq(h',ts,d_ang);                       %DTFT
f = ((0:df1:df1*(length(x1)-1))-fs/2)*0.2*pi;

figure(1);                                              %Plot figure
subplot(2,1,1);
plot(h);
title('Time domain of h[n]');
grid on
xlabel("Sample number (n)")
subplot(2,1,2);
plot(f,fftshift(abs(X1)));
title('Magnitude of DTFT of h[n]');
grid on
xlabel("\omega");
xticks([-pi -0.8*pi -0.6*pi -0.4*pi -0.2*pi 0 0.2*pi 0.4*pi 0.6*pi 0.8*pi pi])
xticklabels({'-\pi','-0.8\pi','-0.6\pi','-0.4\pi','-0.2\pi','0','0.2\pi','0.4\pi','0.6\pi','0.8\pi','\pi'})
ylabel("|X(e^{j\omega})|");



%% DTFT function
function [X,x,df]=fftseq(m,ts,df)
% [M,m1,df]=fftseq(m,ts,df)
% [M,m1,df]=fftseq(m,ts) 不輸入df將以最低解析度計算
% m為欲轉換的信號 ts為取樣間隔時間 df為解析度
fs=1/ts;
if nargin == 2
n1=0;
else
n1=fs/df;                                %n1 = 假設頻域上點數
end
n2=length(m);                            %n2 = 實際時域上點數
n=2^(max(nextpow2(n1),nextpow2(n2)));    %取點數最多的那方並找出最靠近2的冪次方的數字
X=fft(m,n);                              %X為FFT轉換後的信號
x=[m,zeros(1,n-n2)];                     %x為將原信號後端補零使取樣個數n為2的冪次方
df=fs/n;                                 %df為此轉換程式真正使用的解析度
end