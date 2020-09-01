#include<libCacheSim.h>
#include<math.h>
int main(){

    uint64_t unique = 7850256;
    for(int i=10; i<=90; i = i+10){
        reader_init_param_t init_csv = {
            .delimiter=',', 
            .obj_id_field=1,
            .op_field=2,
            .has_header=false
        };
        reader_t *reader_csv = open_trace("./w106.csv", CSV_TRACE, OBJ_ID_NUM, &init_csv);
        request_t  *req = new_request();

        float_t cz = ((float) i / 100.0) * (float) unique;
        printf("The cz is %f\n", cz);
        common_cache_params_t cc_params = {.cache_size=cz};
        
        cache_t *cache = create_cache("LRU", cc_params, NULL);
        cache_t *mru_cache = create_cache("MRU", cc_params, NULL);
        cache_t *fifo_cache = create_cache("FIFO", cc_params, NULL);
        // cache_t *lfu_cache = create_cache("LFU", cc_params, NULL);
        // cache_t *lhd_cache = create_cache("LHD", cc_params, NULL);

        uint64_t hit_count = 0, total_count = 0;
        uint64_t mru_hit_count = 0;
        uint64_t fifo_hit_count = 0;
        uint64_t lfu_hit_count = 0;
        uint64_t lhd_hit_count = 0;

        while(read_one_req(reader_csv, req) == 0){
            if(cache->get(cache, req) != cache_ck_miss){
                hit_count++;
            }
        
            if(mru_cache->get(mru_cache, req) != cache_ck_miss){
                mru_hit_count++;
            }

            if(fifo_cache->get(fifo_cache, req) != cache_ck_miss){
                fifo_hit_count++;
            }

            // if(lfu_cache->get(lfu_cache, req) != cache_ck_miss){
            //     lfu_hit_count++;
            // }

            // if(lhd_cache->get(lhd_cache, req) != cache_ck_miss){
            //     lhd_hit_count++;
            // }

            total_count++;
        }

        printf("The total number of requests is: %ld\n", total_count);

        // printf("The total number of LRU hits for size %f is %ld\n", cz, hit_count);
        // printf("The total number of MRU hits for size %f is %ld\n", cz, mru_hit_count);    

        float fifo_hit_ratio = 100 * ((double) fifo_hit_count / (double) total_count);
        float mru_hit_ratio = 100 * ((double) mru_hit_count / (double) total_count);
        float hit_ratio = 100 * ( (double) hit_count / (double) total_count);
        // float lfu_hit_ratio = 100 * ((double) lfu_hit_count / (double) total_count);
        // float lhd_hit_ratio = 100 * ((double) lhd_hit_count / (double) total_count);

        printf("The LRU hit ratio for size %d is: %f\n", i, hit_ratio);
        printf("The MRU hit ratio for size %d is: %f\n", i, mru_hit_ratio);
        printf("The FIFO hit ratio for size %d is: %f\n", i, fifo_hit_ratio);
        // printf("The LFU hit ratio for size %d is: %f\n", i, lfu_hit_ratio);
        // printf("The LHD hit ratio for size %d is: %f\n", i, lhd_hit_ratio);
        
        cache->cache_free(cache);
        mru_cache->cache_free(mru_cache);
        fifo_cache->cache_free(fifo_cache);
        
        free_request(req);
        close_reader(reader_csv);
    }
    
}