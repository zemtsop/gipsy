/*============================================================================
                                  gdsc_range.c
------------------------------------------------------------------------------

                              COPYRIGHT (c) 1990
                        Kapteyn Astronomical Institute
         University of Groningen  -  9700 AV Groningen, The Netherlands

#> gdsc_range.dc2
Function:      GDSC_RANGE

Purpose:       return lower left and upper right corner of a subset

Category:      GDS

File:          gdsc_range.c

Author:        W. Zwitser

Use:           GDSC_RANGE( SET,                    Input       character
                           SUBSET,                 Input       integer*8
                           CLOW,                   Output      integer*8
                           CUPP,                   Output      integer*8
                           ERROR )                 In/Out      integer

               SET           set name      

               SUBSET        coordinate word of subset

               CLOW          lower left coordinate word

               CUPP          upper right coordinate word

               ERROR         0  = successful
                            <0  = a GDS error

Updates:       Dec  5, 1989: WZ, migrated to C
               Oct 31, 1990: WZ, 'naxis', etc. from setsta -> dsc_file
               Mar 24, 1994: JPT, modified to cooperate with GDS server.
#<

@ integer function gdsc_range( character, 
@                              integer*8,
@                              integer*8,
@                              integer*8,
@                              integer )

----------------------------------------------------------------------------*/

#include    "gdsparams.h"
#include    "gdserrors.h"
#include    "gdsd_basic.h"
#include "userfio.h"

void  gdsc_range_c( fchar     set,           /* name of set                 */
                    fint8     *subset,        /* subset coordinate word      */
                    fint8     *c1,            /* lower left coordinate word  */
                    fint8     *c2,            /* upper right coordinate word */
                    fint     *err )          /* error code                  */
{
   fint        iax, naxis, size;
   fint8       max, min;
   gds_coord *setinfo;

   (void)gds_rhed(set, &setinfo);
   anyoutf(1, "range start: %lld %lld %lld\n", *subset, *c1, *c1);
   *c1 = *subset;
   *c2 = *subset;
   naxis = setinfo->naxis;
   for ( iax = 0; iax < naxis; iax++ ) {
      *err = 0;
      min = gds___unpack_c( set, subset, &iax, err );
      if ( *err == GDS_COORDUNDEF ) {
         *err = 0;
         size = setinfo->size[iax];
         max = min + size - 1;
         gds___pack_c( set, c1, &min, &iax, err );
         gds___pack_c( set, c2, &max, &iax, err );
      }
   }
   anyoutf(1, "range %lld %lld %lld %lld %lld\n", min, max, *c1, *c2);
}
