#pragma once

#include <PolyVox/Region.h>

#include "VoreealRegion.generated.h"

// Defines how the bounding volumes intersects or contain one another.
UENUM(Blueprintable)
enum class EContainmentType : uint8
{
	// Indicates that there is no overlap between two bounding volumes.
	Disjoint,
	// Indicates that one bounding volume completely contains another volume.
	Contains,
	// Indicates that bounding volumes partially overlap one another.
	Intersects
};

/// Defines a region within a volume.
USTRUCT(BlueprintType)
struct VOREEAL_API FVoreealRegion
{
	GENERATED_BODY()

	// Position X component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	// Position Y component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	// Position Z component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Z;

	// Position Width component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width;

	// Position Height component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height;

	// Position Depth component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Depth;

	// Default constructor
	FORCEINLINE FVoreealRegion()
		: X(0), Y(0), Z(0), Width(0), Height(0), Depth(0)
	{ }

	FORCEINLINE FVoreealRegion(int32 X, int32 Y, int32 Z, int32 Width, int32 Height, int32 Depth)
		: X(X), Y(Y), Z(Z), Width(Width), Height(Height), Depth(Depth)
	{ }

	explicit FORCEINLINE FVoreealRegion(FVector Size)
		: FVoreealRegion(FVector::ZeroVector, Size)
	{ }

	FORCEINLINE FVoreealRegion(FVector Lower, FVector Upper)
		: X(Lower.X), Y(Lower.Y), Z(Lower.Z)
		, Width(Lower.X - Upper.X), Height(Lower.Y - Upper.Y), Depth(Lower.Z - Upper.Z)
	{ }

	FORCEINLINE FVoreealRegion(FIntVector Lower, FIntVector Upper)
		: X(Lower.X), Y(Lower.Y), Z(Lower.Z)
		, Width(Lower.X - Upper.X), Height(Lower.Y - Upper.Y), Depth(Lower.Z - Upper.Z)
	{ }

	// Convert PolyVox::Region to FVoreealRegion.
	FORCEINLINE FVoreealRegion(const PolyVox::Region &InRegion)
	{
		FVoreealRegion::operator=(InRegion);
	}
 
	// Gets the upper corner.
	FVector Min() const { return FVector(X, Y, Z); }

	// Gets the lower corner.
	FVector Max() const { return FVector(X + Width, Y + Height, Z + Depth); }

	FIntVector GetUpperInt() const;
	FIntVector GetLowerInt() const;

	FVector GetUpper() const; 
	FVector GetLower() const;

	// Gets the size.
	FVector Size() const { return FVector(Width, Height, Depth); }

	// Gets the center of the region.
	FVector GetCenter() const;

	// Grow the region.
	void Grow(int32 Width, int32 Height, int32 Depth);

	// Grow the region from every direction.
	void GrowUnified(int32 Amount);

	// Shift the upper corner of the region.
	void ShiftUpperCorner(int32 X, int32 Y, int32 Z);

	// Shift the lower corner of the region.
	void ShiftLowerCorner(int32 X, int32 Y, int32 Z);

	// Does this region contain another region.
	static EContainmentType Contains(const FVoreealRegion& Region1, const FVoreealRegion& Region2);

	// Does this region contain a vector.
	static bool Contains(const FVoreealRegion& Region1, const FVector& Vector);

	// Does this region intersect another region.
	static bool Intersect(const FVoreealRegion& Region1, const FVoreealRegion& Region2);

	// Region to string.
	FORCEINLINE FString ToString() const
	{
		return FString::Printf(TEXT("Min=[%s] Max=[%s]"), *Min().ToString(), *Max().ToString());
	}

	FORCEINLINE FVoreealRegion& operator=(const PolyVox::Region& Other)
	{
		int32 width = Other.getWidthInVoxels();
		int32 height = Other.getHeightInVoxels();
		int32 depth = Other.getDepthInVoxels();

		this->X = Other.getLowerX();
		this->Y = Other.getLowerY();
		this->Z = Other.getLowerZ();
		this->Width = width;
		this->Height = height;
		this->Depth = depth;
 
		return *this;
	}

	explicit FORCEINLINE operator PolyVox::Region() const
	{ 
		return PolyVox::Region(X, Y, Z, X + Width, Y + Height, Z + Depth);
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FVoreealRegion& Region)
	{
		Ar << Region.X << Region.Y << Region.Z;
		Ar << Region.Width << Region.Height << Region.Depth;
		return Ar;
	}
};

FORCEINLINE FVector FVoreealRegion::GetUpper() const
{
	return FVector(X + Width, Y + Height, Z + Depth);
}

FORCEINLINE FVector FVoreealRegion::GetLower() const
{
	return FVector(X, Y, Z);
}

FORCEINLINE FIntVector FVoreealRegion::GetUpperInt() const
{
	return FIntVector(X + Width, Y + Height, Z + Depth);
}

FORCEINLINE FIntVector FVoreealRegion::GetLowerInt() const
{
	return FIntVector(X, Y, Z);
}

FORCEINLINE FVector FVoreealRegion::GetCenter() const
{
	return FVector(X + (Width / 2), Y + (Height / 2), Z + (Depth / 2));
}

FORCEINLINE void FVoreealRegion::Grow(int32 pWidth, int32 pHeight, int32 pDepth)
{
	this->X -= pWidth;
	this->Y -= pHeight;
	this->Z -= pDepth;
	this->Width += pWidth * 2;
	this->Height += pHeight * 2;
	this->Depth += pDepth * 2;
}

FORCEINLINE void FVoreealRegion::GrowUnified(int32 Amount)
{
	this->X -= Amount;
	this->Y -= Amount;
	this->Z -= Amount;
	this->Width += Amount * 2;
	this->Height += Amount * 2;
	this->Depth += Amount * 2;
}

FORCEINLINE void FVoreealRegion::ShiftUpperCorner(int32 pX, int32 pY, int32 pZ)
{
	X += pX;
	Y += pY;
	Z += pZ;
	Width -= pX;
	Height -= pY;
	Depth -= pZ;
}

FORCEINLINE void FVoreealRegion::ShiftLowerCorner(int32 pX, int32 pY, int32 pZ)
{
	Width += pX;
	Height += pY;
	Depth += pZ;
}

FORCEINLINE EContainmentType FVoreealRegion::Contains(const FVoreealRegion& Region1, const FVoreealRegion& Region2)
{
	FVector Lower1 = Region1.GetLower();
	FVector Upper1 = Region1.GetUpper();
	FVector Lower2 = Region2.GetLower();
	FVector Upper2 = Region2.GetUpper();

	if (Upper2.X < Lower1.X || Lower2.X > Upper1.X ||
		Upper2.Y < Lower1.Y || Lower2.Y > Upper1.Y ||
		Upper2.Z < Lower1.Z || Lower2.Z > Upper1.Z)
	{
		return EContainmentType::Disjoint;
	}

	if (Lower2.X >= Lower1.X && Upper2.X <= Upper1.X &&
		Lower2.Y >= Lower1.Y && Upper2.Y <= Upper1.Y &&
		Lower2.Z >= Lower1.Z && Upper2.Z <= Upper1.Z)
	{
		return EContainmentType::Contains;
	}

	return EContainmentType::Intersects;
}

FORCEINLINE bool FVoreealRegion::Contains(const FVoreealRegion& Region1, const FVector& Vector)
{
	FVector Lower = Region1.GetLower();
	FVector Upper = Region1.GetUpper();
	if (Vector.X < Lower.X || Vector.X > (Upper.X - 1) ||
		Vector.Y < Lower.Y || Vector.Y > (Upper.Y - 1) ||
		Vector.Z < Lower.Z || Vector.Z > (Upper.Z - 1))
	{
		return false;
	}
	return true;
}

FORCEINLINE bool FVoreealRegion::Intersect(const FVoreealRegion& Region1, const FVoreealRegion& Region2)
{
	return Contains(Region1, Region2) == EContainmentType::Intersects;
}
