// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

namespace PMG
{

	/* Implicitly flags collisions as true */
	/*  */
	/* ::new with Preallocated static buffer ( SBO ) -> Move into actual object though */

	inline FProcMeshSection* AddTriangle(UProceduralMeshComponent& Mesh, int32 MeshIndex, const FVector& Offset)
	{
		TArray<FVector>          Normals    ;
		TArray<FProcMeshTangent> Tangents   ; 
		TArray<FColor>           Colors     ; 

		TArray<FVector2D>        UV0
		{
			FVector2D{ 0.0f, 0.0f },
			FVector2D{ 0.0f, 1.0f },
			FVector2D{ 1.0f, 0.0f }
		};

		TArray<FVector>          Vertices
		{
			FVector{0.0f, 0.0f, 0.0f} +   Offset, // Bottom Left
			FVector{0.0f, 100.0f, 0.0f} + Offset, // Bottom Right
			FVector{100.0f, 0.0f, 0.0f} + Offset  // Top Left
		};

		TArray<int32> Triangles{ 0,1,2 };

		Normals.Init(FVector{0.0f,0.0f,1.0f}, 3);
		Tangents.Init(FProcMeshTangent{ 1.0f, 0.0f, 0.0f }, 3);
		Colors.Init(FColor::Black, 3);

		Mesh.CreateMeshSection(MeshIndex, Vertices, Triangles, Normals, UV0, Colors, Tangents, true);

		return Mesh.GetProcMeshSection(MeshIndex);
	}

	/* Throws? , If so what happens when passing in an invalid mesh proc */
	inline FProcMeshSection* AddQuad(UProceduralMeshComponent& Mesh, int32 MeshIndex, const FVector& Offset)
	{
		TArray<FVector>          Normals; 
		TArray<FProcMeshTangent> Tangents; 
		TArray<FColor>           Colors; 


		TArray<FVector2D>        UV0
		{
			{0.0f, 0.0f},
			{0.0f, 1.0f},
			{1.0f, 0.0f},
			{0.0f, 0.0f},
		};

		/*
			*--------------------*
			|                    |
			|                    |
			|                    |
			|                    |
			|                    |
			|                    |
			|                    |
			|                    |
			*--------------------*
		*/


		TArray<FVector>          Vertices
		{
			FVector{0.0f, 0.0f, 0.0f }      + Offset, // Bottom Left
			FVector{0.0f, 100.0f, 0.0f }    + Offset, // Bottom Right
			FVector{100.0f, 0.0f, 0.0f }    + Offset, // Top Left
			FVector{100.0f, 100.0f, 0.0f }  + Offset  // Top Right
		};

		TArray<int32>          Triangles{2,0,1,1,3,2};

		Normals.Init(FVector{0.0f, 0.0f, 1.0f}, 4);
		Tangents.Init(FProcMeshTangent{ 1.0f, 0.0f, 0.0f }, 4);
		Colors.Init(FColor::Black, 4);
		
		Mesh.CreateMeshSection(MeshIndex, Vertices, Triangles, Normals, UV0, Colors, Tangents, true);
		return Mesh.GetProcMeshSection(MeshIndex);
	}
}
