#include "d3dUtil.h"
ID3D11ShaderResourceView* d3dHelper::CreateTexture2DArraySRV( ID3D11Device*device,ID3D11DeviceContext*context, std::vector<std::wstring>&filenames, DXGI_FORMAT format/* =DXGI_FORMAT_FROM_FILE */, UINT filter/* =D3DX11_FILTER_NONE */, UINT mipFilter/* =D3DX11_FILTER_LINEAR */ )
{
	//单独的从文件加载纹理元素，纹理不会直接被GPU使用(0绑定标志)//
	//仅仅被用来从文件加载图片数据//
	//使用方式是D3D11_USAGE_STAGING，从GPU拷贝到CPU//
	UINT size=filenames.size();
	std::vector<ID3D11Texture2D*>srcTex(size);
	for (UINT i=0;i<size;++i)
	{
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width=D3DX11_FROM_FILE;
		loadInfo.Height=D3DX11_FROM_FILE;
		loadInfo.Depth=D3DX11_FROM_FILE;
		loadInfo.FirstMipLevel=0;
		loadInfo.MipLevels=D3DX11_FROM_FILE;
		loadInfo.Usage=D3D11_USAGE_STAGING;
		loadInfo.BindFlags=0;
		loadInfo.CpuAccessFlags=D3D11_CPU_ACCESS_WRITE|D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags=0;
		loadInfo.Format=format;
		loadInfo.MipFilter=mipFilter;
		loadInfo.pSrcInfo=0;
		HR(D3DX11CreateTextureFromFile(device,filenames[i].c_str(),&loadInfo,0,(ID3D11Resource**)&srcTex[i],0));
	}

	//创建纹理数组，数组中的每个元素都有一样的格式/规格//

	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc=texElementDesc;
	
	ID3D11Texture2D*texArray=0;
	HR(device->CreateTexture2D(&texArrayDesc,0,&texArry));
	//将每个纹理元素拷贝进数组//
	for (UINT texElement=0;texElement<size;++texElement)
	{
		for (UINT mipLevel=0;texElement<size;++texElement)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			HR(context->Map(srcTex[texElement],mipLevel,D3D11_MAP_READ,0,&mappedTex2D));////将srcTex[texElement]内容映射到mappedTex2D，这样有GPU对srcTex[texElement]操作权转换到CPU对mappedTex2D的操作上，mipLevel指定转换的是哪个mip，cpu对mappedTex2D的读写操作直接映射到srcTex[texElement]的内容上，此处设置的是读取内容权限。结束后要unmap掉，使GPU重获读写的操作权限。
			//UpdateSubresource相关可以看：http://blog.csdn.net/a359877454/article/details/79024473 //
			context->UpdateSubresource(texArray,D3D11CalcSubresource(mipLevel,texElement,texElementDesc.MipLevels)//D3D中Buffer实际是按一维存储的（无论是一维，二维，还是3维纹理）
				,0,mappedTex2D.pData,mappedTex2D.RowPitch,mappedTex2D.DepthPitch);
			context->Unmap(srcTex[texElement],mipLevel);//将读取到的纹理存入纹理数组//


		}
	}
	//为纹理数组创建着色器资源视图//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format=texArrayDesc.Format;
	viewDesc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip=0;
	viewDesc.Texture2DArray.MipLevels=texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice=0;
	viewDesc.Texture2DArray.ArraySize=size;
	ID3D11ShaderResourceView*texArraySRV=0;
	HR(device->CreateShaderResourceView(texArray,&viewDesc,&texArraySRV));
	//
	//清理--我们仅需要纹理视图,视图创建后，存储资源的内存可以释放了。//
	//
	ReleaseCOM(texArray);
	for (UINT i=0;i<size;++i)
	{
		ReleaseCOM(srcTex[i]);
	}

	return texArraySRV;

}
ID3D11ShaderResourceView*d3dHelper::CreateRandomTexture1DSRV(ID3D11Device*device)
{

	//创建随机数据//
	XMFLOAT4 randomValues[1024];
	for (int i=0;i<1024;++i)
	{
		randomValues[i].x=MathHelper::RandF(-1.0f,1.0f);
		randomValues[i].y=MathHelper::RandF(-1.0f,1.0f);
		randomValues[i].z=MathHelper::RandF(-1.0f,1.0f);
		randomValues[i].w=MathHelper::RandF(-1.0f,1.0f);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem=randomValues;
	initData.SysMemPitch=1024*sizeof(XMFLOAT4);
	initData.SysMemSlicePitch=0;

	//创建纹理//
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width=1024;
	texDesc.MipLevels=1;
	texDesc.Format=DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage=D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags=D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags=0;
	texDesc.MiscFlags=0;
	texDesc.ArraySize=1;
	ID3D11Texture1D* randomTex=0;
	HR(device->CreateTexture1D(&texDesc,&initData,&randomTex));

	//创建视图//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format=texDesc.Format;
	viewDesc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels=texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip=0;

	ID3D11ShaderResourceView*randomTexSRV=0;
	HR(device->CreateShaderResourceView(randomTex,&viewDesc,&randomTexSRV));

	ReleaseCOM(randomTex);


	return randomTexSRV;

}
//提取平截头体的面//
void ExtractFrustumPlanes(XMFLOAT4 planes[6],CXMMATRIX M)
{

	//
	// Left
	//
	planes[0].x = M(0,3) + M(0,0);
	planes[0].y = M(1,3) + M(1,0);
	planes[0].z = M(2,3) + M(2,0);
	planes[0].w = M(3,3) + M(3,0);

	//
	// Right
	//
	planes[1].x = M(0,3) - M(0,0);
	planes[1].y = M(1,3) - M(1,0);
	planes[1].z = M(2,3) - M(2,0);
	planes[1].w = M(3,3) - M(3,0);

	//
	// Bottom
	//
	planes[2].x = M(0,3) + M(0,1);
	planes[2].y = M(1,3) + M(1,1);
	planes[2].z = M(2,3) + M(2,1);
	planes[2].w = M(3,3) + M(3,1);

	//
	// Top
	//
	planes[3].x = M(0,3) - M(0,1);
	planes[3].y = M(1,3) - M(1,1);
	planes[3].z = M(2,3) - M(2,1);
	planes[3].w = M(3,3) - M(3,1);

	//
	// Near
	//
	planes[4].x = M(0,2);
	planes[4].y = M(1,2);
	planes[4].z = M(2,2);
	planes[4].w = M(3,2);

	//
	// Far
	//
	planes[5].x = M(0,3) - M(0,2);
	planes[5].y = M(1,3) - M(1,2);
	planes[5].z = M(2,3) - M(2,2);
	planes[5].w = M(3,3) - M(3,2);

	//归一化平面方程//
	for (int i=0;i<6;++i)
	{
		XMVECTOR v=XMPlaneNormalize(XMLoadFloat4(&planes[i]));
		XMStoreFloat4(&planes[i],v);
	}
}