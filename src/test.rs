#[cfg(test)]
mod tests {
    use crate::vec2::*;

    #[test]
    fn vec_div() {
        let hor = Vec2{
            x: 1,
            y: 0,
        };

        let ver = Vec2{
            x: 0,
            y: 5,
        };

        let neg_hor = Vec2{
            x: -10,
            y: 0,
        };

        let neg_ver = Vec2{
            x: 0,
            y: -5,
        };

        let diag = Vec2{
            x: 1,
            y: 1,
        };

        let diag2 = Vec2{
            x: 2,
            y: 2,
        };

        assert!((hor * 2).is_div_by(&hor));
        assert!(!(hor * 0).is_div_by(&hor));
        assert!(diag2.is_div_by(&diag));
    }
}

