function f=adpmedian(g,Smax)
%adpmedian  Perform adaptive median filtering
%�����ж�smax�Ƿ����Ҫ��smaxӦ���Ǵ���1������
if    ((Smax<=1)||(Smax/2==round(Smax/2))||(Smax~=round(Smax)))
    error('SMAX must be an odd integer>1 .')
end
%[M,N]=size(g);
%��ʼ��
f=g;
f(:)=0;
alreadyProcessed=false(size(g));
%��ʼ�˲�
for k=3:2:Smax
%�൱��k��k�ļ�Сֵ�˲�
    zmin=ordfilt2(g,1,ones(k,k),'symmetric');
%�൱��k��k�ļ���ֵ�˲�
    zmax=ordfilt2(g,k*k,ones(k,k),'symmetric');
%�൱��k��k����ֵ�˲�
    zmed=medfilt2(g,[k,k],'symmetric');
%�ж�Z med�Ƿ�Ϊһ����
    processUsingLevelB=(zmed>zmin) & (zmax>zmed) & ~alreadyProcessed;
%�ж�Z x��y�Ƿ�Ϊһ����
    zB=(g<zmin) & (zmax>g);
    outputZxy=processUsingLevelB & zB;
    outputZmed=processUsingLevelB & ~zB;
%���һ�����������ֵZ x��y������������ֵ��Ϊ���
    f(outputZxy)=g(outputZxy);
%�����ֵ�˲���ֵ��Ϊ���
    f(outputZmed)=zmed(outputZmed);
    alreadyProcessed=alreadyProcessed|processUsingLevelB;
    if all(alreadyProcessed(:))
        break;
    end
end




