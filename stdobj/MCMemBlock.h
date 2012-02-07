// MCMemBlock.h: interface for the CMCMemBlock class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MCMEMBLOCK_H_
#define _MCMEMBLOCK_H_

#if _MSC_VER > 1000
#pragma once
#endif //_MSC_VER>1000

class CMCMemBlock : public CObject  
{	
public:
	CMCMemBlock(bool bFreeOnDestroy=true);
	virtual ~CMCMemBlock();	
	DWORD	AddMemoryBlock(void* const pBuf,DWORD nLen);
	void	DelMemoryBlock();
	DWORD	GetMemorySize() const;
	void*	GetMemoryPtr() const;	
private:
	bool	m_bFreeOnDestroy;	// true=memory is freed upon object being destroyed.
	BYTE*	m_pBuf;				// pointer to the beginning memory block.	
	DWORD	m_nLen;				// Length of the buffer pointed to by m_pBuf.	
	UINT	m_nBlocks;			// Number of memory blocks we have allocated so far.
public:
	static UINT m_nNumObjects;	// How many instances we have of this object type.
private:
	static UINT m_nStdBlock;	// Standard block size to allocate for memory	
	
};

#endif // #ifndef _MCMEMBLOCK_H_
