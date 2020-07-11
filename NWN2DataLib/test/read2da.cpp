#include <iostream>

#include <Precomp.h>
#include <2DAFileReader.h>

int main( int argc, char* argv[] )
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[ 0 ] << " <CLASSES.2DA file>" << std::endl;
        return 1;
    }

    const auto filepath = argv[ 1 ];
    TwoDAFileReader tda( filepath );

    std::cout << "file: " << filepath << std::endl;
    std::cout << "row count: " << tda.GetRowCount() << std::endl;
    std::cout << "column count: " << tda.GetColumnCount() << std::endl;

    const auto labelColName = "Label";
    const auto hasCol = tda.HasColumn( labelColName );
    std::cout << "has column '" << labelColName << "': " << hasCol << std::endl;

    if( !hasCol ) {
        exit(1);
    }

    const auto nameColName = "Name";
    std::string strVal;
    int intVal;
    for( size_t i = 0 ; i < tda.GetRowCount(); ++i ) {
        if( tda.Get2DAString( labelColName, i, strVal ) ) {
            if( tda.Get2DAInt( nameColName, i, intVal ) ) {
                std::cout << i << " " << strVal << " " << intVal << std::endl;
            }
        }
    }

}
