#include<stdio.h>
#include<libCacheSim.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
    uint64_t read_hit;
    uint64_t write_hit;
    uint64_t read_miss;
    uint64_t write_miss;
    uint64_t read_count;
    uint64_t write_count;
    uint64_t total_count;
    uint64_t total_hit;
    uint64_t total_miss;
}traceStats;

void compute_stat(char* file_name, int size, int percent, char* op_fname){
    reader_init_param_t init_csv = {
        .delimiter=',',
        .obj_id_field=1,
        .op_field=2,
        .has_header=false
    };

    reader_t *reader_csv = open_trace(file_name, CSV_TRACE, OBJ_ID_NUM, &init_csv);
    request_t *req = new_request();

    common_cache_params_t cc_params = {.cache_size=size};

    cache_t *lru_cache = create_cache("LRU", cc_params, NULL);
    traceStats trace_stat = {0,0,0,0,0,0,0};

    while(read_one_req(reader_csv, req) == 0){
        if(lru_cache->get(lru_cache, req) != cache_ck_miss){
            if(req->op == OP_READ){
                trace_stat.read_hit++;
                trace_stat.read_count++;
                trace_stat.total_hit++;
            }else{
                trace_stat.write_hit++;
                trace_stat.write_count++;
                trace_stat.total_hit++;
            }
        }else{
            if(req->op == OP_READ){
                trace_stat.read_miss++;
                trace_stat.read_count++;
                trace_stat.total_miss++;
            }else{
                trace_stat.write_miss++;
                trace_stat.write_count++;
                trace_stat.total_miss++;
            }
        }
        trace_stat.total_count++;
    }

    lru_cache->cache_free(lru_cache);
    free_request(req);
    close_reader(reader_csv);

    // printf("Writing to output file\n");
    FILE *out_file = fopen(op_fname, "a");
    fprintf(out_file, "cache_size,algo,hit_count,miss_count,total_count\n");
    fprintf(out_file, "%ld,LRU,%ld,%ld,%ld\n",size,trace_stat.total_hit,trace_stat.total_miss,trace_stat.total_count);
    fclose(out_file);
}

int main(int argc, char* argv[]){
    
    uint64_t unique_size = atoi(argv[2]);
    compute_stat(argv[1], unique_size, 100, argv[3]);
}