#include "supermode.h"

// Purchase PREMIUM source @Cazzstar Display Name: Thermal5000
// Also selling undetected Fortnite, Apex, Valorant usermodes
// Crypto only ~~ Come money ready

auto main( ) -> int
{
	remove("C:\\indices.json");
	std::printf( "\n [vpm] -> welcome.\n" );

	auto m_image_base = m_physical->get_process_base( "interface.exe" );
	if ( !m_image_base) {
		std::printf( " [vpm] -> failed to get target. %I64d\n", m_image_base);
		std::cin.get( );
	}

	supermode::insert_pte();
	supermode::insert_pfn();
	supermode::insert_target( m_physical.get( )->cr3 );

	supermode::save_indices_for_target();

	// unload driver
	//supermode::wnbios.unload_driver();

	std::printf( "\n [vpm] -> pte-pfn manipulation invoked.\n" );

	return std::cin.get() != EOF ? false : true;
}

// Purchase PREMIUM source @Cazzstar Display Name: Thermal5000
// Also selling undetected Fortnite, Apex, Valorant usermodes
// Crypto only ~~ Come money ready

// code for checking base/cr3 ~~ TASK MANAGER OPEN
/*
	uintptr_t target_base = m_physical->get_process_base( "Taskmgr.exe" );
	std::cout << " base -> " << target_base << std::endl;

	uintptr_t target_cr3 = m_physical->find_dtb_from_base(target_base);
	std::cout << std::hex << target_cr3 << std::endl;
*/