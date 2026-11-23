/*
在BasicTarget基础上增加了可以切换滤波器的功能，详细实现了标准的扩展卡尔曼滤波
*/
class IFilter {
public:
    virtual ~IFilter() = default;
    
    // 预测与更新，传入目标模型的引用，让滤波器去操作目标的状态
    virtual void predict(BasicTarget& target, double dt) = 0;
    virtual void update(BasicTarget& target, const Eigen::VectorXd& z_meas) = 0;
};
// 普通线性卡尔曼滤波
class LKFFilter : public IFilter {
public:
    void predict(BasicTarget& target, double dt) override {}
    void update(BasicTarget& target, const Eigen::VectorXd& z_meas) override {}

};
// 标准的扩展卡尔曼滤波
class EKFFilter : public IFilter {
public:
    void predict(BasicTarget& target, double dt) override {
        Eigen::MatrixXd F = target.F(dt);
        target.setState(F * target.getState());
        target.setCovariance(F * target.getCovariance() * F.transpose() + target.Q(dt));
    }

    void update(BasicTarget& target, const Eigen::VectorXd& z_meas) override {
        Eigen::MatrixXd H = target.H_jacobian();
        Eigen::VectorXd x = target.getState();
        Eigen::MatrixXd P = target.getCovariance();
        
        Eigen::VectorXd z_pred = target.h_predict(x);
        Eigen::VectorXd y = target.z_substract(z_meas, z_pred);
        
        Eigen::MatrixXd S = H * P * H.transpose() + target.getR();
        Eigen::MatrixXd K = P * H.transpose() * S.inverse();
        
        target.setState(x + K * y);
        Eigen::MatrixXd I = Eigen::MatrixXd::Identity(P.rows(), P.cols());
        target.setCovariance((I - K * H) * P);
    }
};

// 迭代扩展卡尔曼滤波
class IEKFFilter : public IFilter {
public:
    void predict(BasicTarget& target, double dt) override { /* 同 EKF */ }
    void update(BasicTarget& target, const Eigen::VectorXd& z_meas) override {
        // 在此实现迭代更新的逻辑（多次线性化）
        // 调用 target.z_substract, target.H_jacobian
    }
};

// 迭代误差状态卡尔曼滤波
class IESKFFilter : public IFilter {
    // IESKF 需要用到流形/误差状态操作，需要 target 提供 boxplus (⊕) 和 boxminus (⊖)
public:
    void predict(BasicTarget& target, double dt) override { /* 误差状态预测 */ }
    void update(BasicTarget& target, const Eigen::VectorXd& z_meas) override {
        // 迭代计算误差状态 delta_x
        // 然后调用 target.boxplus(target.X_, delta_x) 来更新名义状态
    }
};

class BasicTarget {
protected:
    Eigen::VectorXd X_; // 状态
    Eigen::MatrixXd P_; // 协方差
    State_e state_;
    
    // 滤波策略
    std::shared_ptr<IFilter> filter_; 

public:
    explicit BasicTarget(int _dim, StateMachineParams_s _state_params, std::shared_ptr<IFilter> filter)
        : X_(Eigen::VectorXd::Zero(_dim)), 
          P_(Eigen::MatrixXd::Identity(_dim, _dim)),
          state_params_(_state_params),
          filter_(filter) {}

    // 允许运行时动态切换滤波器（例如从 EKF 换成 IEKF）
    void setFilter(std::shared_ptr<IFilter> filter) { filter_ = filter; }

    // 提供访问器供 Filter 使用
    Eigen::VectorXd& getState() { return X_; }
    Eigen::MatrixXd& getCovariance() { return P_; }
    void setState(const Eigen::VectorXd& x) { X_ = x; }
    void setCovariance(const Eigen::MatrixXd& p) { P_ = p; }

    // 纯虚函数保持原样
    virtual Eigen::MatrixXd F(double _dt) const = 0;
    virtual Eigen::MatrixXd Q(double _dt) const = 0;
    virtual Eigen::MatrixXd H_jacobian(int _id) const = 0;
    virtual Eigen::VectorXd h_predict(const Eigen::VectorXd& _x) const = 0; 
    virtual Eigen::VectorXd z_substract(Eigen::VectorXd _z_meas, Eigen::VectorXd _z_pred) const {
        return _z_meas - _z_pred;
    }

    // --- 针对 IESKF 需要的流形操作 (如果是普通向量，直接加减即可) ---
    virtual Eigen::VectorXd boxplus(const Eigen::VectorXd& state, const Eigen::VectorXd& delta) const {
        return state + delta; // 如果状态是四元数等流形，派生类需重写此函数
    }
    virtual Eigen::VectorXd boxminus(const Eigen::VectorXd& s1, const Eigen::VectorXd& s2) const {
        return s1 - s2;
    }

    // 通用业务逻辑：直接委托给 filter_ 执行
    void predict(double dt) {
        if (filter_) filter_->predict(*this, dt);
    }

    void update(const Eigen::VectorXd& z_meas) {
        if (filter_) filter_->update(*this, z_meas);
    }
};