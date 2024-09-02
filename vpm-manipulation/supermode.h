#pragma once
#include "drv.h"
#include <iostream>
#include <unordered_map>
#include "json.hpp"

// Purchase PREMIUM source @Cazzstar Display Name: Thermal5000
// Also selling undetected Fortnite, Apex, Valorant usermodes
// Crypto only ~~ Come money ready

namespace supermode
{
	typedef union _virt_addr_t
	{
		std::uintptr_t value;
		struct
		{
			std::uint64_t offset : 12;
			std::uint64_t pt_index : 9;
			std::uint64_t pd_index : 9;
			std::uint64_t pdpt_index : 9;
			std::uint64_t pml4_index : 9;
			std::uint64_t reserved : 16;
		};
	} virt_addr_t, * pvirt_addr_t;
	static_assert( sizeof( virt_addr_t ) == sizeof( void* ), "Size mismatch, only 64-bit supported." );

	typedef union _pml4e
	{
		std::uint64_t value;
		struct
		{
			std::uint64_t present : 1;          // Must be 1, region invalid if 0.
			std::uint64_t writeable : 1;        // If 0, writes not allowed.
			std::uint64_t user_supervisor : 1;   // If 0, user-mode accesses not allowed.
			std::uint64_t PageWriteThrough : 1; // Determines the memory type used to access PDPT.
			std::uint64_t page_cache : 1; // Determines the memory type used to access PDPT.
			std::uint64_t accessed : 1;         // If 0, this entry has not been used for translation.
			std::uint64_t Ignored1 : 1;
			std::uint64_t large_page : 1;         // Must be 0 for PML4E.
			std::uint64_t Ignored2 : 4;
			std::uint64_t pfn : 36; // The page frame number of the PDPT of this PML4E.
			std::uint64_t Reserved : 4;
			std::uint64_t Ignored3 : 11;
			std::uint64_t nx : 1; // If 1, instruction fetches not allowed.
		};
	} pml4e, * ppml4e;
	static_assert( sizeof( pml4e ) == sizeof( void* ), "Size mismatch, only 64-bit supported." );

	typedef union _pdpte
	{
		std::uint64_t value;
		struct
		{
			std::uint64_t present : 1;          // Must be 1, region invalid if 0.
			std::uint64_t rw : 1;        // If 0, writes not allowed.
			std::uint64_t user_supervisor : 1;   // If 0, user-mode accesses not allowed.
			std::uint64_t PageWriteThrough : 1; // Determines the memory type used to access PD.
			std::uint64_t page_cache : 1; // Determines the memory type used to access PD.
			std::uint64_t accessed : 1;         // If 0, this entry has not been used for translation.
			std::uint64_t Ignored1 : 1;
			std::uint64_t large_page : 1;         // If 1, this entry maps a 1GB page.
			std::uint64_t Ignored2 : 4;
			std::uint64_t pfn : 36; // The page frame number of the PD of this PDPTE.
			std::uint64_t Reserved : 4;
			std::uint64_t Ignored3 : 11;
			std::uint64_t nx : 1; // If 1, instruction fetches not allowed.
		};
	} pdpte, * ppdpte;
	static_assert( sizeof( pdpte ) == sizeof( void* ), "Size mismatch, only 64-bit supported." );

	typedef union _pde
	{
		std::uint64_t value;
		struct
		{
			std::uint64_t present : 1;          // Must be 1, region invalid if 0.
			std::uint64_t rw : 1;        // If 0, writes not allowed.
			std::uint64_t user_supervisor : 1;   // If 0, user-mode accesses not allowed.
			std::uint64_t PageWriteThrough : 1; // Determines the memory type used to access PT.
			std::uint64_t page_cache : 1; // Determines the memory type used to access PT.
			std::uint64_t accessed : 1;         // If 0, this entry has not been used for translation.
			std::uint64_t Ignored1 : 1;
			std::uint64_t large_page : 1; // If 1, this entry maps a 2MB page.
			std::uint64_t Ignored2 : 4;
			std::uint64_t pfn : 36; // The page frame number of the PT of this PDE.
			std::uint64_t Reserved : 4;
			std::uint64_t Ignored3 : 11;
			std::uint64_t nx : 1; // If 1, instruction fetches not allowed.
		};
	} pde, * ppde;
	static_assert( sizeof( pde ) == sizeof( void* ), "Size mismatch, only 64-bit supported." );

	typedef union _pte
	{
		std::uint64_t value;
		struct
		{
			std::uint64_t present : 1;          // Must be 1, region invalid if 0.
			std::uint64_t rw : 1;        // If 0, writes not allowed.
			std::uint64_t user_supervisor : 1;   // If 0, user-mode accesses not allowed.
			std::uint64_t PageWriteThrough : 1; // Determines the memory type used to access the memory.
			std::uint64_t page_cache : 1; // Determines the memory type used to access the memory.
			std::uint64_t accessed : 1;         // If 0, this entry has not been used for translation.
			std::uint64_t Dirty : 1;            // If 0, the memory backing this page has not been written to.
			std::uint64_t PageAccessType : 1;   // Determines the memory type used to access the memory.
			std::uint64_t Global : 1;           // If 1 and the PGE bit of CR4 is set, translations are global.
			std::uint64_t Ignored2 : 3;
			std::uint64_t pfn : 36; // The page frame number of the backing physical page.
			std::uint64_t reserved : 4;
			std::uint64_t Ignored3 : 7;
			std::uint64_t ProtectionKey : 4;  // If the PKE bit of CR4 is set, determines the protection key.
			std::uint64_t nx : 1; // If 1, instruction fetches not allowed.
		};
	} pte, * ppte;
	static_assert( sizeof( pte ) == sizeof( void* ), "Size mismatch, only 64-bit supported." );

	struct pt_entries
	{
		std::pair<ppml4e, pml4e>	pml4;
		std::pair<ppdpte, pdpte>	pdpt;
		std::pair<ppde, pde>		pd;
		std::pair<ppte, pte>		pt;
	};

	enum PAGING_STAGE
	{
		PML4,
		PDPT,
		PD,
		PT
	};

	uint64_t mal_pointer_pte_ind[ 4 ];
	uint64_t mal_pte_ind[ 4 ];
	uint64_t mal_pml4_pte_ind[ 4 ];

	uint64_t mal_pointer_pte_struct[ 4 ];
	uint64_t mal_pte_struct[ 4 ];
	uint64_t mal_pml4_pte_struct[ 4 ];

	struct PTE_PFN
	{
		uint64_t pfn;
		uint64_t offset;
	};
	PTE_PFN pte_pfn;

	void valid_pml4e( uint64_t* pml4ind, uint64_t* pdptstruct )
	{
		if ( !m_physical.get( )->attached_proc || !m_physical.get( )->cr3 )
		{
			std::printf( " [VPM - Thermal] -> not attached to any process.\n" );
			return;
		}

		// find a valid entry
		for ( int i = 1; i < 512; i++ )
		{
			pml4e m_pml4e;
			if ( !m_physical.get()->read_physical_memory( ( m_physical.get( )->cr3 + i * sizeof( uintptr_t ) ), &m_pml4e, sizeof( pml4e ) ) )
			{
				std::printf( " [VPM - Thermal] -> read failed.\n" );
				return;
			}

			// page backs physical memory
			if ( m_pml4e.present && m_pml4e.user_supervisor )
			{
				std::printf( " [pml4(index)] -> %i\n", i );
				*pml4ind = i;
				*pdptstruct = m_pml4e.pfn * 0x1000;
				return;
			}
		}
	}

	void valid_pdpte( uint64_t pdptstruct, uint64_t* pdpteind, uint64_t* pdstruct )
	{
		if ( !m_physical.get( )->attached_proc || !m_physical.get( )->cr3 )
		{
			std::printf( " [VPM - Thermal] -> not attached to any process.\n" );
			return;
		}

		// find a valid entry
		for ( int i = 0; i < 512; i++ )
		{
			pdpte m_pdpte;
			if ( !m_physical.get( )->read_physical_memory( ( pdptstruct + i * sizeof( uintptr_t ) ), &m_pdpte, sizeof( pdpte ) ) )
			{
				std::printf( " [VPM - Thermal] -> read failed.\n" );
				return;
			}

			// page backs physical memory
			if ( m_pdpte.present && m_pdpte.user_supervisor )
			{
				std::printf( " [pdpte(index)] -> %i\n", i );
				*pdpteind = i;
				*pdstruct = m_pdpte.pfn * 0x1000;
				return;
			}
		}
	}

	void valid_pde( uint64_t pdstruct, uint64_t* pdind, uint64_t* ptstruct )
	{
		if ( !m_physical.get( )->attached_proc || !m_physical.get( )->cr3 )
		{
			std::printf( " [VPM - Thermal] -> not attached to any process.\n" );
			return;
		}

		// find a valid entry
		for ( int i = 0; i < 512; i++ )
		{
			pde m_pde;
			if ( !m_physical.get( )->read_physical_memory( ( pdstruct + i * sizeof( uintptr_t ) ), &m_pde, sizeof( pde ) ) )
			{
				std::printf( " [VPM - Thermal] -> read failed.\n" );
				return;
			}

			// page backs physical memory
			if ( m_pde.present && m_pde.user_supervisor )
			{
				std::printf( " [pde(index)] -> %i\n", i );
				*pdind = i;
				*ptstruct = m_pde.pfn * 0x1000;
				return;
			}
		}
	}

	void free_pte( uint64_t ptstruct, uint64_t* ptind )
	{
		if ( !m_physical.get( )->attached_proc || !m_physical.get( )->cr3 )
		{
			std::printf( " [VPM - Thermal] -> not attached to any process.\n" );
			return;
		}

		// find a valid entry
		for ( int i = 0; i < 512; i++ )
		{
			pte m_pte;
			if ( !m_physical.get( )->read_physical_memory( ( ptstruct + i * sizeof( uintptr_t ) ), &m_pte, sizeof( pte ) ) )
			{
				std::printf( " [VPM - Thermal] -> read failed.\n" );
				return;
			}

			if ( !m_pte.present )
			{
				std::printf( " [free(index)] -> %i\n", i );
				*ptind = i;
				return;
			}
		}
	}

	PTE_PFN calc_pfnpte_from_addr( uint64_t addr )
	{
		PTE_PFN pte_pfn;
		uint64_t pfn = addr >> 12;
		pte_pfn.pfn = pfn;
		pte_pfn.offset = addr - ( pfn * 0x1000 );
		return pte_pfn;
	}

	uint64_t generate_virtual_address( uint64_t pml4, uint64_t pdpt, uint64_t pd, uint64_t pt, uint64_t offset )
	{
		uint64_t virtual_address =
			( pml4 << 39 ) |
			( pdpt << 30 ) |
			( pd << 21 ) |
			( pt << 12 ) |
			offset;

		return virtual_address;
	}

	void change_mal_pt_pfn( uint64_t pfn )
	{
		pte mal_pte;
		mal_pte.present = 1;
		mal_pte.rw = 1;
		mal_pte.user_supervisor = 1;
		mal_pte.pfn = pfn;
		mal_pte.nx = 1;
		mal_pte.Dirty = 1;
		mal_pte.accessed = 1;

		uint64_t va = generate_virtual_address( mal_pointer_pte_ind[ PML4 ], mal_pointer_pte_ind[ PDPT ], mal_pointer_pte_ind[ PD ], mal_pointer_pte_ind[ PT ], 0 );
		memcpy( ( void* ) va, &mal_pte, sizeof( pte ) );
	}

	void read_physical_memory( uint64_t addr, uint64_t size, uint64_t* buf )
	{
		PTE_PFN pfn = calc_pfnpte_from_addr( addr );
		change_mal_pt_pfn( pfn.pfn );

		uint64_t va = generate_virtual_address( mal_pte_ind[ PML4 ], mal_pte_ind[ PDPT ], mal_pte_ind[ PD ], mal_pte_ind[ PT ], pfn.offset );
		memcpy( ( void* ) buf, ( void* ) va, size );
	}

	// this pte will point to physical mem
	auto insert_pte( ) -> void
	{
		valid_pml4e( &mal_pte_ind[ PML4 ], &mal_pte_struct[ PDPT ] );
		valid_pdpte( mal_pte_struct[ PDPT ], &mal_pte_ind[ PDPT ], &mal_pte_struct[ PD ] );
		valid_pde( mal_pte_struct[ PD ], &mal_pte_ind[ PD ], &mal_pte_struct[ PT ] );
		free_pte( mal_pte_struct[ PT ], &mal_pte_ind[ PT ] );

		std::cout << "\n [pml4] -> " << mal_pte_ind[ PML4 ] << std::endl
			<< " [pdpt] -> " << mal_pte_ind[ PDPT ] << std::endl
			<< " [pd] -> " << mal_pte_ind[ PD ] << std::endl
			<< " [pte] -> " << mal_pte_ind[ PT ] << std::endl;

		pte m_pte;
		m_pte.present = 1;
		m_pte.rw = 1;
		m_pte.user_supervisor = 1;
		m_pte.pfn = 0;
		m_pte.nx = 1;
		m_pte.Dirty = 1;
		m_pte.accessed = 1;

		auto pte_physical = mal_pte_struct[ PT ] + mal_pte_ind[ PT ] * sizeof( uintptr_t );
		pte_pfn = calc_pfnpte_from_addr( pte_physical );

		m_physical.get( )->write_physical_memory( pte_physical, &m_pte, sizeof( pte ) );
		std::cout << " [pte(index] -> " << mal_pte_ind[ PT ] << " [" << pte_physical << "] " << std::dec << std::endl;
	}

	// this pte will point to our other malicious pte we can modify from our UM program then
	void insert_pfn( )
	{
		valid_pml4e( &mal_pointer_pte_ind[ PML4 ], &mal_pointer_pte_struct[ PDPT ] );
		valid_pdpte( mal_pointer_pte_struct[ PDPT ], &mal_pointer_pte_ind[ PDPT ], &mal_pointer_pte_struct[ PD ] );
		valid_pde( mal_pointer_pte_struct[ PD ], &mal_pointer_pte_ind[ PD ], &mal_pointer_pte_struct[ PT ] );
		free_pte( mal_pointer_pte_struct[ PT ], &mal_pointer_pte_ind[ PT ] );

		std::cout << "\n [pml4] -> " << mal_pointer_pte_ind[ PML4 ] << std::endl
			<< " [pdpt] -> " << mal_pointer_pte_ind[ PDPT ] << std::endl
			<< " [pd] -> " << mal_pointer_pte_ind[ PD ] << std::endl
			<< " [pte] -> " << mal_pointer_pte_ind[ PT ] << std::endl;

		pte m_pte;
		m_pte.present = 1;
		m_pte.rw = 1;
		m_pte.user_supervisor = 1;
		m_pte.pfn = pte_pfn.pfn;
		m_pte.nx = 1;
		m_pte.Dirty = 1;
		m_pte.accessed = 1;

		uintptr_t pte_physical = mal_pointer_pte_struct[ PT ] + mal_pointer_pte_ind[ PT ] * sizeof( uintptr_t );

		m_physical.get( )->write_physical_memory( pte_physical, &m_pte, sizeof( pte ) );
		std::cout << " [pfn(index] " << mal_pointer_pte_ind[ PT ] << " [" << std::hex << pte_physical << "] " << " pointing to pfn " << pte_pfn.pfn << std::endl;
	}

	void insert_target( uintptr_t target_pml4 )
	{
		std::cout << "inserting third pte for dbt: " << std::hex << target_pml4 << std::dec << std::endl;

		// find a free pte and populate other indices while at it
		valid_pml4e( &mal_pml4_pte_ind[ PML4 ], &mal_pml4_pte_struct[ PDPT ] );
		valid_pdpte( mal_pml4_pte_struct[ PDPT ], &mal_pml4_pte_ind[ PDPT ], &mal_pml4_pte_struct[ PD ] );
		valid_pde( mal_pml4_pte_struct[ PD ], &mal_pml4_pte_ind[ PD ], &mal_pml4_pte_struct[ PT ] );
		free_pte( mal_pml4_pte_struct[ PT ], &mal_pml4_pte_ind[ PT ] );

		std::cout << "PML4: " << mal_pml4_pte_ind[ PML4 ] << std::endl
			<< "PDPT: " << mal_pml4_pte_ind[ PDPT ] << std::endl
			<< "PD: " << mal_pml4_pte_ind[ PD ] << std::endl
			<< "PT: " << mal_pml4_pte_ind[ PT ] << std::endl;

		auto pml4_pte_pfn = calc_pfnpte_from_addr( target_pml4 );

		pte mal_pte;
		mal_pte.present = 1;
		mal_pte.rw = 1;
		mal_pte.user_supervisor = 1;
		mal_pte.pfn = pml4_pte_pfn.pfn;
		mal_pte.nx = 1;
		mal_pte.Dirty = 1;
		mal_pte.accessed = 1;

		uintptr_t mal_pte_phys = mal_pml4_pte_struct[ PT ] + mal_pml4_pte_ind[ PT ] * sizeof( uintptr_t );

		m_physical.get( )->write_physical_memory( mal_pte_phys, &mal_pte, sizeof( pte ) );
		std::cout << "inserted third malicious pte at index " << mal_pml4_pte_ind[ PT ] << " [" << std::hex << mal_pte_phys << "] " << "pointing to pfn " << pml4_pte_pfn.pfn << " offset: " << pml4_pte_pfn.offset << std::dec << std::endl;
	}

	// goofy ahh
	void save_indices_for_target( )
	{
		nlohmann::json j;
		j[ "mal_pte_indices" ] = {};
		j[ "mal_pointer_pte_indices" ] = {};
		j[ "mal_pml4_pte_indices" ] = {};

		for ( int i = 0; i <= PT; i++ )
		{
			j[ "mal_pte_indices" ][ std::to_string( i ) ] = mal_pte_ind[ i ];
			j[ "mal_pointer_pte_indices" ][ std::to_string( i ) ] = mal_pointer_pte_ind[ i ];
			j[ "mal_pml4_pte_indices" ][ std::to_string( i ) ] = mal_pml4_pte_ind[ i ];
		}

		j[ "cr3" ] = m_physical.get( )->get_system_dirbase( );

		std::ofstream out_file( "C:\\indices.json" );
		out_file << j.dump( );
	}
}