clear;
close all;
I=dicomread('dicomfile.dcm'); 
X=double(I);
%A=25*I;
%imshow(A);
subplot(1,2,1);
%[]���Զ�ѡ��һ���������й�һ����������ʾһ��ͼ��
imshow(I,[]); title('ֱ�Ӵ�dicom');
info=dicominfo('dicomfile.dcm');
wincenter=info.WindowCenter;
winwidth=info.WindowWidth;
width=info.Width;
height=info.Height;
A=zeros(height,width);
for i=1:height
    for j=1:width
        if X(i,j)>(wincenter+winwidth/2)
            A(i,j)=255;
        elseif X(i,j)<(wincenter-winwidth/2)
            A(i,j)=0;
        else
            A(i,j)=(X(i,j)-(wincenter-winwidth /2))*256/winwidth;
        end;
    end;
end;
%A=(I-(wincenter-width /2))*(255 /winwidth );
%imshow(A);
disp('wincenter=');disp(wincenter);
disp('winwidth=');disp(winwidth);
disp('width=');disp(width);
disp('height=');disp(height);
%A=A*255;
A=A/255;   %��һ��
subplot(1,2,2);
imshow(A);title('ͨ����λ�����dicom');