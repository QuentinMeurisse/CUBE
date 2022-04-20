#ifndef CUBE_SEGMENTTREE_H
#define CUBE_SEGMENTTREE_H

#include "../redblacktree.h"
#include "../segment.h"

template<class Comparator = std::less_equal<Segment>>
class SegmentTree : public RedBlackTree<Segment, Comparator> {
public:
    /**
     *
     * @param s We suppose that s is contained in a segment in our tree and it doesn't intersect the other
     * @return the segment containing s
     */
    std::shared_ptr<TreeNode<Segment>> search_segment_containing(Segment const& s) {
        return search_segment_helper(this->root, s);
    }

private:
    std::shared_ptr<TreeNode<Segment>> search_segment_helper( const std::shared_ptr<TreeNode<Segment>>& n, Segment const& s){
        if (n == nullptr)
            return nullptr;
        else {
            if (s.is_subsegment_of(n->data))
                return n;
            else if (Comparator()(s, n->data) and n->left != nullptr)
                return search_segment_helper(n->left, s);
            else if (not Comparator()(s, n->data) and n->right != nullptr)
                return search_segment_helper(n->right, s);
            else
                return nullptr;
        }
    }

};


#endif //CUBE_SEGMENTTREE_H
