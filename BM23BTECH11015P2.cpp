#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <unordered_set>
#include <algorithm>
using namespace std;

// Structure for a Post
struct Post {
    int id;
    int userId;
    string content;
    int likes;
    int comments;
    int timestamp;

    Post(int id, int userId, string content, int timestamp)
        : id(id), userId(userId), content(content), likes(0), comments(0), timestamp(timestamp) {}
};

// Graph-based relationships between users
class SocialGraph {
public:
    map<int, unordered_set<int>> friendships;

    void addFriendship(int userA, int userB) {
        friendships[userA].insert(userB);
        friendships[userB].insert(userA);
    }

    const unordered_set<int>& getFriends(int userId) {
        return friendships[userId];
    }
};

// Social Media Feed Algorithm
class SocialMediaFeed {
private:
    vector<Post> posts;
    SocialGraph graph;

    struct PostScore {
        int score;
        Post post;

        bool operator<(const PostScore& other) const {
            return score < other.score;
        }
    };

public:
    void addPost(int userId, string content, int timestamp) {
        posts.emplace_back(posts.size(), userId, content, timestamp);
    }

    void addFriendship(int userA, int userB) {
        graph.addFriendship(userA, userB);
    }

    void likePost(int postId) {
        if (postId >= 0 && postId < posts.size()) {
            posts[postId].likes++;
        }
    }

    void commentOnPost(int postId) {
        if (postId >= 0 && postId < posts.size()) {
            posts[postId].comments++;
        }
    }

    vector<Post> getFeed(int userId) {
        priority_queue<PostScore> pq;

        for (const Post& post : posts) {
            int score = 0;

            // Base score from interactions
            score += post.likes * 10;
            score += post.comments * 20;

            // Bonus if post is from a friend
            if (graph.getFriends(userId).count(post.userId)) {
                score += 50;
            }

            // Recency bonus (smaller timestamps are older)
            score += max(0, 100 - (currentTime() - post.timestamp));

            pq.push({score, post});
        }

        vector<Post> feed;
        while (!pq.empty() && feed.size() < 10) { // Top 10 posts
            feed.push_back(pq.top().post);
            pq.pop();
        }
        return feed;
    }

private:
    int currentTime() {
        return 1000; // Simulate a constant current time for simplicity
    }
};

int main() {
    SocialMediaFeed smFeed;

    // Add users and friendships
    smFeed.addFriendship(1, 2);
    smFeed.addFriendship(1, 3);

    // Add posts
    smFeed.addPost(1, "Post from User 1", 950);
    smFeed.addPost(2, "Post from User 2", 980);
    smFeed.addPost(3, "Post from User 3", 970);

    // Interactions
    smFeed.likePost(0);
    smFeed.commentOnPost(1);
    smFeed.likePost(2);
    smFeed.commentOnPost(2);

    // Get feed for User 1
    vector<Post> feed = smFeed.getFeed(1);

    cout << "User 1's Feed:\n";
    for (const Post& post : feed) {
        cout << "Post ID: " << post.id
             << ", User ID: " << post.userId
             << ", Content: " << post.content
             << ", Likes: " << post.likes
             << ", Comments: " << post.comments
             << endl;
    }

    return 0;
}
