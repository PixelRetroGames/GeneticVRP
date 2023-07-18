import matplotlib.pyplot as plt


def plot_sol(depot, nodes, truck_paths, title, name):
	for truck_idx, truck_path in enumerate(truck_paths):
		truck_line_x = [depot[0]]
		truck_line_y = [depot[1]]
		
		for node in truck_path:
			truck_line_x.append(nodes[node - 1][0])
			truck_line_y.append(nodes[node - 1][1])
		
		truck_line_x.append(depot[0])
		truck_line_y.append(depot[1])
		
		plt.plot(truck_line_x, truck_line_y, label = f"Truck #{truck_idx + 1}")

	nodes_x, nodes_y = zip(*nodes)

	plt.scatter(nodes_x, nodes_y, color = "blue", label = "Drop-off point")
	plt.scatter([depot[0]], [depot[1]], color = "orange", label = "Depot")
	plt.title(title)
	plt.legend()
	plt.savefig(name)
	plt.clf()


def optimal_sol_1():
	depot = (145, 215)
	nodes = [(151, 264), (159, 261), (130, 254), (128, 252), (163, 247), (146, 246), (161, 242), (142, 239), (163, 236), (148, 232), (128, 231), (156, 217), (129, 214), (146, 208), (164, 208), (141, 206), (147, 193), (164, 193), (129, 189), (155, 185), (139, 182)]
	truck_paths = [[17, 20, 18, 15, 12], [16, 19, 21, 14], [13, 11, 4, 3, 8, 10], [9, 7, 5, 2, 1, 6]]
	plot_sol(depot, nodes, truck_paths, "Optimal solution for E-n22-k4", "optimal_1.png")
	

def subopt_sol_1():
	depot = (145, 215)
	nodes = [(151, 264), (159, 261), (130, 254), (128, 252), (163, 247), (146, 246), (161, 242), (142, 239), (163, 236), (148, 232), (128, 231), (156, 217), (129, 214), (146, 208), (164, 208), (141, 206), (147, 193), (164, 193), (129, 189), (155, 185), (139, 182)]
	truck_paths = [[10, 8, 3, 4, 16], [14, 18, 15, 9, 7, 11, 13], [19, 21, 20, 17], [12, 5, 2, 1, 6]]
	plot_sol(depot, nodes, truck_paths, "Suboptimal solution for E-n22-k4", "subopt_1.png")

def best_sol_1():
	depot = (145, 215)
	nodes = [(151, 264), (159, 261), (130, 254), (128, 252), (163, 247), (146, 246), (161, 242), (142, 239), (163, 236), (148, 232), (128, 231), (156, 217), (129, 214), (146, 208), (164, 208), (141, 206), (147, 193), (164, 193), (129, 189), (155, 185), (139, 182)]
	truck_paths = [[10, 7, 5, 9, 12], [14, 17, 21, 19, 13], [15, 18, 20, 16], [11, 4, 3, 1, 2, 6, 8]]
	plot_sol(depot, nodes, truck_paths, "Best found solution for E-n22-k4", "bestsol_1.png")

def main():
	optimal_sol_1()
	subopt_sol_1()
	best_sol_1()
	

if __name__ == "__main__":
	main()
