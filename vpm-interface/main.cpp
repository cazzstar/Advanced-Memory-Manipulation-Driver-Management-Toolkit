#include "rwptm.h"

// Purchase PREMIUM source @Cazzstar Display Name: Thermal5000
// Also selling undetected Fortnite, Apex, Valorant usermodes
// Crypto only ~~ Come money ready

auto main( ) -> int
{

	// json
	while (!supermode_comm::load())
	{
		std::printf( " [VPM - Thermal] -> waiting on parse.\n" );
		Sleep(1000);
	}

	// inti r/w
	auto init = rwptm::init("Taskmgr.exe", "interface.exe");
	if ( !init )
	{
		std::printf( " [access] -> failed.\n" );
	}
	std::printf( " [access] -> valid.\n" );

	/*
	auto instance = rwptm::read_virtual_memory<std::uintptr_t>( rwptm::target_base );
	if ( !instance ) {
		std::printf( " [read] -> buffer is null.\n" );
		std::cin.get( );
	}
	std::printf( " [buffer] -> %I64d\n", instance );*/

	//const auto rpm_time = [ & ] ( ) -> float
	//{
	//	auto time_now = std::chrono::high_resolution_clock::now( );

	//	for ( auto x = 0ull; x < 0x10000; x++ )
	//	{

	//		auto value = rwptm::read_virtual_memory<std::uintptr_t>( rwptm::target_base + x );
	//		if ( !value )
	//			continue;

	//	}

	//	auto time_span = std::chrono::duration_cast< std::chrono::duration< float > >(
	//		std::chrono::high_resolution_clock::now( ) - time_now
	//	);

	//	return time_span.count( );
	//};
	//std::printf( " [read(0x10,000)] -> %fs\n", rpm_time( ) );

	//const auto rpm_time2 = [ & ] ( ) -> float
	//{
	//	auto time_now = std::chrono::high_resolution_clock::now( );
	//	for ( auto x = 0ull; x < 0x100000; x++ )
	//	{

	//		auto value = rwptm::read_virtual_memory<std::uintptr_t>( rwptm::target_base + x );
	//		if ( !value )
	//			continue;
	//	}
	//	auto time_span =
	//		std::chrono::duration_cast< std::chrono::duration< float> >( std::chrono::high_resolution_clock::now( ) - time_now );
	//	return time_span.count( );
	//};
	//std::printf( " [read(0x100,000)] -> %fs\n", rpm_time2( ) );

	return std::cin.get( ) != EOF ? false : true;
}