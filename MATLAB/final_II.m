%��dicomͼ��
clear;
close all;
I=imread('ultrasound.bmp'); 
D=double(I);
figure;
imshow(uint8(D));
title(' ԭʼͼ�� ');

%����Ӧ��ֵ�˲�ȥ�뷽��
salt=imnoise(I,'salt & pepper',0.25);    %���������ܶ�0.25
figure;
subplot(1,2,1);
imshow(salt);
title(' �ӽ�������ͼ�� ');
%�Լ�����ͼ�������ֵ�˲����˲�����[7 7]
medfilt2=Medfilt2(salt,[7 7],'symmetric');
subplot(1,2,2);
imshow(medfilt2);
title(' ��ͨ��ֵ�˲�ȥ��ͼ�� ');
%�Լ�����ͼ���������Ӧ��ֵ�˲����˲���󴰿�[7 7]
method1=adpmedian(salt,7);
figure;
imshow(method1);
title(' ����Ӧ��ֵ�˲�ȥ��ͼ��');

%С���任��ֵȥ�뷽��
init = 2055615866; randn('seed',init);
gaussian = D + 20*randn(size(D));                %���������׼��20
figure;
subplot(1,2,1);
imshow(uint8(gaussian));
title(' ����˹����ͼ�� ');
[thr,sorh,keepapp]=ddencmp('den','wv',gaussian);
%disp('thr=');disp(thr);
wavelet=wdencmp('gbl',gaussian,'sym4',2,thr,sorh,keepapp);   %sym8С������Ȼ����
subplot(1,2,2);
imshow(uint8(wavelet));
title('С���任�����ͼ��');
%��ͼ�����3���άС���ֽ�
[c,s]=wavedec2(gaussian,3,'sym4');                                    %sym8С������Ȼ����
[thr3,nkeep3]=wdcbm2(c,s,1.5,2.7*prod(s(1,:)));
%disp('thr3=');disp(thr3);
%disp('nkeep3=');disp(nkeep3);
[method2,cxd,sxd,perf0,perf12]=wdencmp('lvd',c,s,'sym8',3,thr3,'s');   %sym8С������Ȼ����
figure;
imshow(uint8(method2));
title('ʹ�÷ֲ���ֵ����ͼ��');

%��������ȥ�뷽��
num_iter = 15;
delta_t = 1/7;
kappa = 30;
option = 2;
method3 = anisodiff2D(gaussian,num_iter,delta_t,kappa,option);
figure;
imshow(uint8(method3));
title(' �������Խ����ͼ�� ');

%���ܲ���
%���齵��ͼ��ͬԭͼ������
err1=norm(wavelet-D);
err2=norm(method2-D);
%�����������ɷ�
per1=norm(wavelet)/norm(gaussian);
per2=norm(method2)/norm(gaussian);
disp('err1=');disp(err1);
disp('err2=');disp(err2);
disp('per1=');disp(per1);
disp('per2=');disp(per2);
%�����ͼ��ͽ���ǰͼ�������

