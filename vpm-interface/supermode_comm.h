#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <thread>
#include "json.hpp"

// Purchase PREMIUM source @Cazzstar Display Name: Thermal5000
// Also selling undetected Fortnite, Apex, Valorant usermodes
// Crypto only ~~ Come money ready

namespace supermode_comm
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

	static uint64_t mal_pointer_pte_ind[4];
	static uint64_t mal_pte_ind[4];
	static uint64_t mal_pml4_pte_ind[4];

	static uint64_t system_cr3;
	static uint64_t target_cr3;

	static uint64_t current_pfn = 0;

	// decryption table key = pml4 encrypted pfn val = pml4 decrypted pfn
	static std::unordered_map<int, int> decryption_table;

	struct PTE_PFN
	{
		uint64_t pfn;
		uint64_t offset;
	};
	static PTE_PFN mal_pte_pfn;
	
	// rudamentary ahhhh data transmition
	static bool load()
	{
		std::ifstream ifs("C:\\indices.json");
		if ( !ifs.is_open( ) )
			return false;

		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		nlohmann::json j = nlohmann::json::parse(content);

		system_cr3 = j["cr3"].get<uint64_t>();
		std::cout << "system_cr3: 0x" << std::hex << system_cr3 << std::dec << std::endl;

		for (int i = 0; i <= PT; i++)
		{
			mal_pointer_pte_ind[i] = j["mal_pointer_pte_indices"][std::to_string(i)].get<uint64_t>();
		}
		for (int i = 0; i <= PT; i++)
		{
			if (mal_pte_ind[i] == 0)
				mal_pte_ind[i] = j["mal_pte_indices"][std::to_string(i)].get<uint64_t>();
		}
		for (int i = 0; i <= PT; i++)
		{
			if (mal_pml4_pte_ind[i] == 0)
				mal_pml4_pte_ind[i] = j["mal_pml4_pte_indices"][std::to_string(i)].get<uint64_t>();
		}

		return true;
	}

	static PTE_PFN calc_pfnpte_from_addr(uint64_t addr)
	{
		PTE_PFN pte_pfn;
		uint64_t pfn = addr >> 12;
		pte_pfn.pfn = pfn;
		pte_pfn.offset = addr - (pfn * 0x1000);
		return pte_pfn;
	}

	static uint64_t generate_virtual_address(uint64_t pml4, uint64_t pdpt, uint64_t pd, uint64_t pt, uint64_t offset)
	{
		uint64_t virtual_address =
			(pml4 << 39) |
			(pdpt << 30) |
			(pd << 21) |
			(pt << 12) |
			offset;

		return virtual_address;
	}

	static void invalidate_pte_tlb()
	{
		uint64_t mal_pte_va = generate_virtual_address(mal_pte_ind[PML4], mal_pte_ind[PDPT], mal_pte_ind[PD], mal_pte_ind[PT], 0);

		while (true)
		{
			__try
			{
				if (*(PCHAR)mal_pte_va)
					continue;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return;
			}

			YieldProcessor();
		}
	}

	static uintptr_t change_mal_pt_pfn(uint64_t pfn)
	{
		current_pfn = pfn;

		uint64_t va = generate_virtual_address(mal_pointer_pte_ind[PML4], mal_pointer_pte_ind[PDPT], mal_pointer_pte_ind[PD], mal_pointer_pte_ind[PT], 0);

		pte mal_pte;
		mal_pte.value = 0;
		memcpy((void*)va, &mal_pte, sizeof(pte));

		invalidate_pte_tlb();
		 
		mal_pte.pfn = pfn;
		mal_pte.present = 1;
		mal_pte.user_supervisor = 1;
		mal_pte.page_cache = 1;
		mal_pte.rw = 1;
		mal_pte.nx = 1;
		mal_pte.accessed = 1;
		mal_pte.Dirty = 1;

		memcpy((void*)va, &mal_pte, sizeof(pte));
		return va;
	}

	static uint64_t dec_pml4e(uint64_t m_pml4e)
	{
		uint64_t va = generate_virtual_address(mal_pml4_pte_ind[PML4], mal_pml4_pte_ind[PDPT], mal_pml4_pte_ind[PD], mal_pml4_pte_ind[PT], m_pml4e * sizeof( pml4e* ));

		pml4e pml4;
		memcpy(&pml4, (void*)va, sizeof( pml4e ));
		uint64_t fake_pfn = pml4.pfn;

		pml4.pfn = decryption_table[fake_pfn + m_pml4e];
		memcpy((void*)va, &pml4, sizeof( pml4e ));

		return fake_pfn;
	}

	static void enc_pml4e(uint64_t m_pml4e, uint64_t fake_pfn)
	{
		uint64_t va = generate_virtual_address(mal_pml4_pte_ind[PML4], mal_pml4_pte_ind[PDPT], mal_pml4_pte_ind[PD], mal_pml4_pte_ind[PT], m_pml4e * sizeof( pml4e* ));

		pml4e pml4;
		memcpy(&pml4, (void*)va, sizeof( pml4e ));
		pml4.pfn = fake_pfn;
		memcpy((void*)va, &pml4, sizeof( pml4e ));
	}

	static bool read_physical_memory(uint64_t addr, uint64_t size, uint64_t* buf)
	{
		PTE_PFN pfn = calc_pfnpte_from_addr(addr);

		if (pfn.pfn != current_pfn)
			change_mal_pt_pfn(pfn.pfn);

		MemoryFence();

		uint64_t va = generate_virtual_address(mal_pte_ind[PML4], mal_pte_ind[PDPT], mal_pte_ind[PD], mal_pte_ind[PT], pfn.offset);

		memcpy((void*)buf, (void*)va, size);
		return true;
	}

	static void write_physical_memory(uint64_t addr, uint64_t size, uint64_t* data)
	{
		PTE_PFN pfn = calc_pfnpte_from_addr(addr);

		if (pfn.pfn != current_pfn)
		{
			change_mal_pt_pfn(pfn.pfn);
			MemoryFence();
		}

		uint64_t va = generate_virtual_address(mal_pte_ind[PML4], mal_pte_ind[PDPT], mal_pte_ind[PD], mal_pte_ind[PT], pfn.offset);
		memcpy((void*)va, (void*)data, size);
	}
}