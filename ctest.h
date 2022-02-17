#include <stdio.h>
#include <stdlib.h>

static int UnitTestPassCount = 0;
static int UnitTestFailCount = 0;

static int UnitTestInit( const char* Title )
{
    printf( "%s\n=================================\n", Title );
    UnitTestPassCount = UnitTestFailCount = 0;
    return 1;
}

static int UnitTestSummary()
{
    printf( "=================================\n"
            "Pass : %3d\nFail : %3d\n",
            UnitTestPassCount, UnitTestFailCount );
    return 0;
}

#define UnitTest( Title ) \
    for( int ONCE = UnitTestInit( Title ); ONCE-- || UnitTestSummary(); )

#define CheckIf( Correct ) ( ( Correct ) ? " " : "*" )

#define Expect( Arg, ... )                     \
    do {                                       \
        if( Arg )                              \
            ++UnitTestPassCount;               \
        else                                   \
            ++UnitTestFailCount;               \
        if( ! ( Arg ) || 1 )                   \
        {                                      \
            printf( "[%s] ", CheckIf( Arg ) ); \
            __VA_OPT__( __VA_ARGS__ );         \
            printf( #Arg "\n" );               \
        }                                      \
    } while( 0 )

#define TraceParam( Format, Arg ) printf( "[ " #Arg " = " Format " ] ", Arg )

#define TraceLine() printf( "[ >_%d ] ", __LINE__ )

#define EXPAND( ... )  EXPAND1( EXPAND1( EXPAND1( EXPAND1( __VA_ARGS__ ) ) ) )
#define EXPAND1( ... ) EXPAND2( EXPAND2( EXPAND2( EXPAND2( __VA_ARGS__ ) ) ) )
#define EXPAND2( ... ) __VA_ARGS__

#define PAREN            ()
#define FOR_EACH( ... )  EXPAND( FOR_EACH_IMPL( __VA_ARGS__ ) )
#define FOR_EACH_TOKEN() FOR_EACH_IMPL
#define FOR_EACH_IMPL( F, a, ... ) \
    F( a ) __VA_OPT__(, FOR_EACH_TOKEN PAREN( F, __VA_ARGS__ ) )

#define StripeParen_( ... ) __VA_ARGS__
#define StripeParen( ARGS ) StripeParen_ ARGS

#define EExpect( F, ARGS, Ped, ... )                                             \
    do {                                                                         \
        printf( "[%s] ", CheckIf( F ARGS Ped ) );                                \
        printf( #F __VA_OPT__( "(" __VA_ARGS__ ") " #Ped "\n"), StripeParen_ ARGS ); \
    } while( 0 )

// EExpect( MidPoint, ( 10, 20 ), == 14 );

////////////////////////////////////////////////////////////////////////////////////

#define DONT_RUN_DEMO_
#ifndef DONT_RUN_DEMO

int MidPoint( int a, int b ) { return a / 2 + b / 2; }

void MidPoint_UnitTest()
{
    UnitTest( "MidPoint(int,int)" )
    {
        Expect( MidPoint( 0, 0 ) == 0, TraceLine() );
        Expect( MidPoint( 2, 2 ) == 2, TraceLine() );
        Expect( MidPoint( 2147483645, 51 ) == 1073741848, TraceLine() );
        Expect( MidPoint( 2147483646, 50 ) == 1073741848, TraceLine() );

        for( int k = -1; k < 2; k += 2 )
            for( int l = -1; l < 2; l += 2 )
                for( int i = 0; i < 32; ++i )
                    for( int j = 0; j < 32; ++j )
                    {
                        int a = k * ( 1 << i ) - 1;
                        int b = l * ( 1 << j ) - 1;
                        Expect( MidPoint( a, b ) == ( a + b ) / 2,
                                TraceParam( "%3d", a ), TraceParam( "%3d", b ) );
                    }
    }
}

////////////////////////////////////////////////////////////////////////////////////

int main()
{
    for( int k = 0; k < 20; ++k )
        EExpect( MidPoint, ( k, 20 ), == 14, "%d, %d" );
    // MidPoint_UnitTest();
    return 0;
}

#endif