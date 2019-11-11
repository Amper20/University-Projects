package parallelTree;

import java.util.concurrent.CyclicBarrier;
/**
 * @author cristian.chilipirea
 *
 */
public class TreeNode {
	int name;
	public TreeNode left = null;
	public TreeNode right = null;

	TreeNode(int name) {
		this.name = name;
	}
	void addChild(TreeNode child) {
		
		synchronized (this){
			if (left == null) {
				left = child;
				return;
			}
			right = child;
			
		}
	}
	
	TreeNode getNode(int name) {
		TreeNode aux = null;
		if (this.name == name)
			return this;
		if (left != null)
			aux = left.getNode(name);
		if (aux != null)
			return aux;
		if (right != null)
			aux = right.getNode(name);
		if (aux != null)
			return aux;
		return null;
	}
}