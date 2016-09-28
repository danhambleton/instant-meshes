#include "batch.h"
#include "serializer.h"
#include <thread>
#include <cstdlib>
#include <vector>

int nprocs = -1;

int main(int argc, char **argv) 
{
 	std::vector<std::string> args;
    bool extrinsic = true, dominant = false, align_to_boundaries = false;
    bool fullscreen = false, help = false, deterministic = false, compat = false;
    int rosy = 4, posy = 4, face_count = -1, vertex_count = -1;
    uint32_t knn_points = 10, smooth_iter = 2;
    Float crease_angle = -1, scale = -1;
    std::string batchOutput;

	if(argc >= 2)
		args.push_back(argv[1]);

	//TODO:parse command line
	batchOutput = "out.obj";

    int nConstraints = 0;
    nConstraints += scale > 0 ? 1 : 0;
    nConstraints += face_count > 0 ? 1 : 0;
    nConstraints += vertex_count > 0 ? 1 : 0;

    if (nConstraints > 1) {
        cerr << "Error: Only one of the --scale, --face and --vertices parameters can be used at once!" << endl;
        help = true;
    }

    tbb::task_scheduler_init init(nprocs == -1 ? tbb::task_scheduler_init::automatic : nprocs);

    if (!batchOutput.empty() && args.size() == 1) {
        try {
            batch_process(args[0], batchOutput, rosy, posy, scale, face_count,
                          vertex_count, crease_angle, extrinsic,
                          align_to_boundaries, smooth_iter, knn_points,
                          !dominant, deterministic);
            return 0;
        } catch (const std::exception &e) {
            cerr << "Caught runtime error : " << e.what() << endl;
            return -1;
        }
    }
}