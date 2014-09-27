%��dicomͼ��
clear;
close all;
I=dicomread('dicomfile.dcm'); 
X=double(I);
%A=25*I;
%imshow(A);
%[]���Զ�ѡ��һ���������й�һ����������ʾһ��ͼ��
%imshow(I,[]);
info=dicominfo('dicomfile.dcm');
wincenter=info.WindowCenter;
winwidth=info.WindowWidth;
width=info.Width;
height=info.Height;
D=zeros(height,width);
for i=1:height
    for j=1:width
        if X(i,j)>(wincenter+winwidth/2)
            D(i,j)=254;
        elseif X(i,j)<(wincenter-winwidth/2)
            D(i,j)=1;
        else
            D(i,j)=(X(i,j)-(wincenter-winwidth /2))*256/winwidth;
        end;
    end;
end;
%D=D/255;
E=D/255;
figure;
imshow(uint8(D));
title(' ԭʼͼ�� ');

%����Ӧ��ֵ�˲�ȥ�뷽��
salt=imnoise(E,'salt & pepper',0.25);    %���������ܶ�0.25
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
[method2,cxd,sxd,perf0,perf12]=wdencmp('lvd',c,s,'sym4',3,thr3,'s');   %sym8С������Ȼ����
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
temp1=0;temp2=0;
for i=1:height
    for j=1:width
        temp1=temp1+gaussian(i,j)*gaussian(i,j);
        temp2=temp2+(gaussian(i,j)-D(i,j))*(gaussian(i,j)-D(i,j));
    end;
end;
SNR1=10*log10(double(temp1))-10*log10(double(temp2));
P1=10*log10(double(temp2));
PSNR1=10*log10(255*255)+10*log10(double(height*width))-P1;    %��˹����
temp1=0;temp2=0;
for i=1:height
    for j=1:width
        temp1=temp1+method2(i,j)*method2(i,j);
        temp2=temp2+(method2(i,j)-D(i,j))*(method2(i,j)-D(i,j));
    end;
end;
SNR2=10*log10(double(temp1))-10*log10(double(temp2));
P2=10*log10(double(temp2));
PSNR2=10*log10(255*255)+10*log10(double(height*width))-P2;      %С��ȥ��
temp1=0;temp2=0;
for i=1:height
    for j=1:width
        temp1=temp1+method3(i,j)*method3(i,j);
        temp2=temp2+(method3(i,j)-D(i,j))*(method3(i,j)-D(i,j));
    end;
end;
SNR3=10*log10(double(temp1))-10*log10(double(temp2));
P3=10*log10(double(temp2));
PSNR3=10*log10(255*255)+10*log10(double(height*width))-P3;    %��������ȥ��

temp1=0;
for i=1:height
    for j=1:width
        if method1(i,j)==E(i,j)
            temp1=temp1+1;
        end;
    end;
end;
d=double(temp1/height);
density=1-d/double(width);                                  %����Ӧ��ֵ�˲������ܶ�

temp1=0;
for i=1:height
    for j=1:width
        if medfilt2(i,j)==E(i,j)
            temp1=temp1+1;
        end;
    end;
end;
 d1=double(temp1/height);
density1=1-d1/double(width);                            %��ͨ��ֵ�˲������ܶ�

temp1=0;temp2=0;
for i=1:height
    for j=1:width
        temp1=temp1+salt(i,j)*salt(i,j);
        temp2=temp2+(salt(i,j)-E(i,j))*(salt(i,j)-E(i,j));
    end;
end;
SNR4=10*log10(double(temp1))-10*log10(double(temp2));
P4=10*log10(double(temp2));
PSNR4=10*log10(255*255)+10*log10(double(height*width))-P4;        %��������ͼ��

temp1=0;temp2=0;
for i=1:height
    for j=1:width
        temp1=temp1+medfilt2(i,j)*medfilt2(i,j);
        temp2=temp2+(medfilt2(i,j)-E(i,j))*(medfilt2(i,j)-E(i,j));
    end;
end;
SNR5=10*log10(double(temp1))-10*log10(double(temp2));
P5=10*log10(double(temp2));
PSNR5=10*log10(255*255)+10*log10(double(height*width))-P5;        %��ͨ��ֵ�˲�

temp1=0;temp2=0;
for i=1:height
    for j=1:width
        temp1=temp1+method1(i,j)*method1(i,j);
        temp2=temp2+(method1(i,j)-E(i,j))*(method1(i,j)-E(i,j));
    end;
end;
SNR6=10*log10(double(temp1))-10*log10(double(temp2));
P6=10*log10(double(temp2));
PSNR6=10*log10(255*255)+10*log10(double(height*width))-P6;       %����Ӧ��ֵ�˲�

disp('����������ͼ�������');disp(SNR4);
disp('����������ͼ���ֵ�����');disp(PSNR4);
disp('����Ӧ��ֵ�˲�ȥ���ͼ�������');disp(SNR6);
disp('����Ӧ��ֵ�˲�ȥ���ͼ���ֵ�����');disp(PSNR6);
disp('����Ӧ��ֵ�˲�ȥ���ͼ�������ܶ�');disp(density);
disp('��ֵ�˲�ȥ���ͼ�������');disp(SNR5);
disp('��ֵ�˲�ȥ���ͼ���ֵ�����');disp(PSNR5);
disp('��ֵ�˲�ȥ���ͼ�������ܶ�');disp(density1);
disp('����˹����ͼ�������');disp(SNR1);
disp('����˹����ͼ���ֵ�����');disp(PSNR1);
disp('С��ȥ���ͼ�������');disp(SNR2);
disp('С��ȥ���ͼ���ֵ�����');disp(PSNR2);
disp('��������ȥ���ͼ�������');disp(SNR3);
disp('��������ȥ���ͼ���ֵ�����');disp(PSNR3);
