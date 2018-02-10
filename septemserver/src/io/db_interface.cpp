#include "io/db_interface.h"

#include <iostream>


    jb = ejdbnew();
    if (!ejdbopen(jb, "addressbook",
                  JBOWRITER | JBOCREAT | JBOTRUNC)) {
        return 1;
    }
    //Get or create collection 'contacts'
    EJCOLL *coll = ejdbcreatecoll(jb, "contacts", NULL);

    bson bsrec;
    bson_oid_t oid;

    //Insert one record:
    //JSON: {'name' : 'Bruce', 'phone' : '333-222-333', 'age' : 58}
    bson_init(&bsrec);
    bson_append_string(&bsrec, "name", "Bruce");
    bson_append_string(&bsrec, "phone", "333-222-333");
    bson_append_int(&bsrec, "age", 58);
    bson_append_date(&bsrec,"uploaddate",(bson_date_t)time(0)); 
    
    bson_finish(&bsrec);
    //Save BSON
    ejdbsavebson(coll, &bsrec, &oid);
    fprintf(stderr, "\nSaved Bruce");
    bson_destroy(&bsrec);

    //Now execute query
    //QUERY: {'name' : {'$begin' : 'Bru'}}
    //Name starts with 'Bru' string

    bson bq1;
    bson_init_as_query(&bq1);
    bson_append_start_object(&bq1, "name");
    bson_append_string(&bq1, "$begin", "Bru");
    bson_append_finish_object(&bq1);
    bson_finish(&bq1);

    EJQ *q1 = ejdbcreatequery(jb, &bq1, NULL, 0, NULL);

    uint32_t count;
    TCLIST *res = ejdbqryexecute(coll, q1, &count, 0, NULL);
    fprintf(stderr, "\n\nRecords found: %d\n", count);

    //Now print the result set records
    for (int i = 0; i < TCLISTNUM(res); ++i) {
        bson *bsdata = (bson*)TCLISTVALPTR(res, i);
        /*
        //if(bson_iterator_type(res)==bson_date) bson_date_t date_it = bson_iterator_date( res ); 
        
        bson *bsdata = (bson*)TCLISTVALPTR(res, i);
        bson_timestamp_t ts;
        bson_iterator it;
        bson_iterator_init( &it , bsdata );
        if( bson_iterator_next( &it ) )
        {
            bson_type t = bson_iterator_type( &it );
        
            if( t == BSON_DATE )
            {
                //time_t t = time(0);   // get time now
                //struct tm * now = localtime( & t );
               // struct tm * newtime = bsdata;//localtime( & t );
                //int64_t out;
               // bson_little_endian64(&out, bson_iterator_value( &it ));
               int64_t raw_time = bson_iterator_long_raw( &it);
                //cout << (now->tm_year + 1900) << '-' 
               //      << (now->tm_mon + 1) << '-'
               //      <<  now->tm_mday
                //     << endl;
                //time_t t = time(0);   // get time now
                struct tm * now = localtime( (time_t*)&raw_time );
                std::cout << (now->tm_year + 1900) << '-' 
                     << (now->tm_mon + 1) << '-'
                     <<  now->tm_mday
                     << std::endl;
            }
        }
         */

        print_raw((char*)bsdata, 0);
    }
    fprintf(stderr, "\n");

    //Dispose result set
    tclistdel(res);

    //Dispose query
    ejdbquerydel(q1);
    bson_destroy(&bq1);

    //Close database
    ejdbclose(jb);
    ejdbdel(jb);
    return 0;
}