// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "MetalRHIPrivate.h"

FMetalShaderResourceView::~FMetalShaderResourceView()
{
	SourceVertexBuffer = NULL;
	SourceTexture = NULL;
}

void FMetalUnorderedAccessView::Set(uint32 ResourceIndex)
{
	// figure out which one of the resources we need to set
	FMetalStructuredBuffer* StructuredBuffer = SourceStructuredBuffer.GetReference();
	FMetalVertexBuffer* VertexBuffer = SourceVertexBuffer.GetReference();
	FRHITexture* Texture = SourceTexture.GetReference();
	if (StructuredBuffer)
	{
		[FMetalManager::GetComputeContext() setBuffer:StructuredBuffer->Buffer offset:0 atIndex:ResourceIndex];
	}
	else if (VertexBuffer)
	{
		[FMetalManager::GetComputeContext() setBuffer:VertexBuffer->Buffer offset:VertexBuffer->Offset atIndex:ResourceIndex];
	}
	else if (Texture)
	{
		FMetalSurface* Surface = GetMetalSurfaceFromRHITexture(Texture);
		if (Surface != nullptr)
		{
			[FMetalManager::GetComputeContext() setTexture:Surface->Texture atIndex:ResourceIndex];
		}
		else
		{
			[FMetalManager::GetComputeContext() setTexture:nil atIndex:ResourceIndex];
		}
	}

}


FUnorderedAccessViewRHIRef FMetalDynamicRHI::RHICreateUnorderedAccessView(FStructuredBufferRHIParamRef StructuredBufferRHI, bool bUseUAVCounter, bool bAppendBuffer)
{
	DYNAMIC_CAST_METALRESOURCE(StructuredBuffer,StructuredBuffer);

	// create the UAV buffer to point to the structured buffer's memory
	FMetalUnorderedAccessView* UAV = new FMetalUnorderedAccessView;
	UAV->SourceStructuredBuffer = StructuredBuffer;

	return UAV;
}

FUnorderedAccessViewRHIRef FMetalDynamicRHI::RHICreateUnorderedAccessView(FTextureRHIParamRef TextureRHI)
{
	FMetalSurface* Surface = GetMetalSurfaceFromRHITexture(TextureRHI);

	// create the UAV buffer to point to the structured buffer's memory
	FMetalUnorderedAccessView* UAV = new FMetalUnorderedAccessView;
	UAV->SourceTexture = (FRHITexture*)TextureRHI;

	return UAV;
}

FUnorderedAccessViewRHIRef FMetalDynamicRHI::RHICreateUnorderedAccessView(FVertexBufferRHIParamRef VertexBufferRHI, uint8 Format)
{
	DYNAMIC_CAST_METALRESOURCE(VertexBuffer, VertexBuffer);

	// create the UAV buffer to point to the structured buffer's memory
	FMetalUnorderedAccessView* UAV = new FMetalUnorderedAccessView;
	UAV->SourceVertexBuffer = VertexBuffer;

	return UAV;
}

FShaderResourceViewRHIRef FMetalDynamicRHI::RHICreateShaderResourceView(FStructuredBufferRHIParamRef StructuredBufferRHI)
{
	DYNAMIC_CAST_METALRESOURCE(StructuredBuffer, StructuredBuffer);

	FMetalShaderResourceView* SRV = new FMetalShaderResourceView;
	return SRV;
}

FShaderResourceViewRHIRef FMetalDynamicRHI::RHICreateShaderResourceView(FVertexBufferRHIParamRef VertexBufferRHI, uint32 Stride, uint8 Format)
{
	DYNAMIC_CAST_METALRESOURCE(VertexBuffer, VertexBuffer);

	FMetalShaderResourceView* SRV = new FMetalShaderResourceView;
	SRV->SourceVertexBuffer = VertexBuffer;
	return SRV;
}

FShaderResourceViewRHIRef FMetalDynamicRHI::RHICreateShaderResourceView(FTexture2DRHIParamRef Texture2DRHI, uint8 MipLevel)
{
	FMetalShaderResourceView* SRV = new FMetalShaderResourceView;
	SRV->SourceTexture = (FRHITexture*)Texture2DRHI;
	return SRV;
}

FShaderResourceViewRHIRef FMetalDynamicRHI::RHICreateShaderResourceView(FTexture2DRHIParamRef Texture2DRHI, uint8 MipLevel, uint8 NumMipLevels, uint8 Format)
{
	FMetalShaderResourceView* SRV = new FMetalShaderResourceView;
	SRV->SourceTexture = (FRHITexture*)Texture2DRHI;
	return SRV;
}

void FMetalDynamicRHI::RHIClearUAV(FUnorderedAccessViewRHIParamRef UnorderedAccessViewRHI, const uint32* Values)
{
	DYNAMIC_CAST_METALRESOURCE(UnorderedAccessView, UnorderedAccessView);

}


